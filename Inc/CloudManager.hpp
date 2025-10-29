#ifndef INC_CLOUDMANAGER_HPP_
#define INC_CLOUDMANAGER_HPP_

#include <string>
#include <functional>
#include <memory>
#include <mqtt/async_client.h>

class CloudManager 
{
public:
    using CommandHandler = std::function<void(const std::string& topic, const std::string& payload)>;

    CloudManager(const std::string& brokerUri,
                 const std::string& clientId,
                 const std::string& username = "",
                 const std::string& password = "");
    ~CloudManager();

    void connect();
    void disconnect();
    bool isConnected() const;
    void publish(const std::string& topic = "",
                const std::string& payload = "",
                int qos = 1,
                bool retained = false);

    void subscribe(const std::string& topic, int qos = 1);
    void setCommandHandler(CommandHandler handler);

private:
    std::string _brokerUri;
    std::string _clientId;
    std::string _username;
    std::string _password;

    mqtt::async_client _client;
    mqtt::connect_options _connOpts;

    CommandHandler _commandHandler;

    // Internal callbacks
    class Callback : public virtual mqtt::callback {
    public:
        explicit Callback(CloudManager& mgr) : _manager(mgr) {}
        void connection_lost(const std::string& cause) override;
        void message_arrived(mqtt::const_message_ptr msg) override;
        void delivery_complete(mqtt::delivery_token_ptr) override {}
    private:
        CloudManager& _manager;
    };

    Callback _callback;
    void onMessageArrived(mqtt::const_message_ptr msg);
    void onConnectionLost(const std::string& cause);
};

#endif // INC_CLOUDMANAGER_HPP_