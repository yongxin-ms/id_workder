#pragma once

#include <mutex>
#include <atomic>
#include <chrono>
#include <exception>
#include <sstream>

/**
 * @brief 分布式id生成类
 * 64bit id: 0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
 *           ||                                                           ||     ||     |  |              |
 *           |└---------------------------时间戳--------------------------┘└中心-┘└机器-┘  └----序列号----┘
 *           |
 *         不用
 * SnowFlake的优点: 整体上按照时间自增排序, 并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分), 并且效率较高, 经测试, SnowFlake每秒能够产生26万ID左右.
 */

namespace id_worker {
	class IdWorker {
	public:
		IdWorker(uint64_t datacenterId, uint64_t workerId) {
			id_.nId = 0;
			id_.stId.workerId = workerId;
			id_.stId.datacenterId = datacenterId;
		}

		/**
		 * 获得下一个ID (该方法是线程安全的)
		 *
		 * @return SnowflakeId
		 */
		uint64_t CreateId() {
			auto timestamp = GetCurMilliSeconds();

			// 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
			if (timestamp < lastTimestamp_) {
				std::ostringstream s;
				s << "clock moved backwards.  Refusing to generate id for " << lastTimestamp_ - timestamp << " milliseconds";
				throw std::exception(std::runtime_error(s.str()));
			}

			std::unique_lock<std::mutex> lock{ mutex_ };
			if (lastTimestamp_ == timestamp) {
				// 如果是同一时间生成的，则进行毫秒内序列
				id_.stId.sequence++;
				if (0 == id_.stId.sequence) {
					// 毫秒内序列溢出, 阻塞到下一个毫秒,获得新的时间戳
					timestamp = WaitTillNextMilliSeconds(lastTimestamp_);
				}
			} else {
				id_.stId.sequence = 0;
			}

			lastTimestamp_ = timestamp;
			id_.stId.timestamp = timestamp - START_TIME;
			return id_.nId;
		}

	protected:

		/**
		 * 返回以毫秒为单位的当前时间
		 *
		 * @return 当前时间(毫秒)
		 */
		int64_t GetCurMilliSeconds() const {
			auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
			return t.time_since_epoch().count();
		}

		/**
		 * 阻塞到下一个毫秒，直到获得新的时间戳
		 *
		 * @param lastTimestamp 上次生成ID的时间截
		 * @return 当前时间戳
		 */
		int64_t WaitTillNextMilliSeconds(int64_t lastTimestamp) const {
			auto timestamp = GetCurMilliSeconds();
			while (timestamp <= lastTimestamp) {
				timestamp = GetCurMilliSeconds();
			}
			return timestamp;
		}

	private:

		union UnionId {
			struct StId {
				uint64_t sequence : 12;
				uint64_t workerId : 5;
				uint64_t datacenterId : 5;
				uint64_t timestamp : 42;
			} stId;
			uint64_t nId;
		};

		const int64_t START_TIME = 1514736000000;//开始时间截 (2018-01-01 00:00:00.000)

		std::mutex mutex_;
		int64_t lastTimestamp_ = 0;
		UnionId id_;
	};
}
