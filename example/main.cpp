#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include "id_worker.h"

int main() {

	enum ENUM_MSG_TYPE {
		MSG_TYPE_NONE			= 0,	//未知
		MSG_TYPE_EVENT			= 1,	//网络事件
		MSG_TYPE_CLIENT			= 2,	//客户端
		MSG_TYPE_GATE			= 3,	//网关
		MSG_TYPE_RANK_SERVER	= 4,	//排行榜服务器
		MSG_TYPE_CENTER_SERVER	= 5,	//中央服务器
		MSG_TYPE_BALANCE_SERVER = 6,	//负载均衡服务器
		MSG_TYPE_COUNT,
	};

	auto id_worker_ptr = std::make_shared<id_worker::IdWorker>(MSG_TYPE_BALANCE_SERVER, 1);

// 	auto tStart = time(nullptr);
// 	const int TOTAL_COUNT = 40000000;
// 	for (int i = 0; i < TOTAL_COUNT; i++) {
// 		id_worker_ptr->CreateId();
// 	}
// 
// 	auto tEnd = time(nullptr);
// 	if (tEnd - tStart > 0) {
// 		auto qps = TOTAL_COUNT / (tEnd - tStart);
// 		printf("qps:%llu\n", qps);
// 	}
// 
	std::vector<uint64_t> vRet;
	for (int i = 0; i < 30; i++) {
		auto id = id_worker_ptr->CreateId();
		printf("0x%016llx\n", id);
		vRet.push_back(id);

		if (i == 10)
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	return 0;
}
