#include <stdio.h>
#include <string>
#include <memory>
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

	auto id_worker_ptr = std::make_shared<id_worker::IdWorker>();
	id_worker_ptr->setDatacenterId(MSG_TYPE_BALANCE_SERVER);
	id_worker_ptr->setWorkerId(1);

	for (int i = 0; i < 10; i++) {
		auto session_id = id_worker_ptr->getId();
		printf("%llu\n", session_id);
	}

	return 0;
}
