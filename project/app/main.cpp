#include <mirai/mirai.h> // 包含此头文件来使用所有 Mirai++ 的 API

int main()
{
    using namespace mirai::literals; // 拉入 _uid _gid 等字面量运算符
    mirai::Session sess("put auth key here", 123456789_uid); // 创建并授权 bot session
    sess.config({}, true); // 开启 WebSocket
    sess.subscribe_messages([&](const mirai::Event& event) // 监听所有消息接受事件
    {
        event.dispatch([&](const mirai::GroupMessage& e) // 当收到的消息是群消息时分发到此函数
        {
            sess.send_message(e.sender.group.id, e.message.content); // 向消息源的群发送一模一样的消息（复读）
        });
    }, mirai::error_logger, mirai::ExecutionPolicy::thread_pool); // 设定异常处理函数为日志输出，执行策略为使用线程池
    std::cin.get(); // 按回车终止程序
    return 0;
}
