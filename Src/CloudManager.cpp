#include "CloudManager.hpp"

#include <iostream>

CloudManager::CloudManager(const std::string& brokerUri,
                           const std::string& clientId,
                           const std::string& username,
                           const std::string& password)
    : _brokerUri(brokerUri),
      _clientId(clientId),
      _username(username),
      _password(password),
      _client(_brokerUri, _clientId),
      _callback(*this)
{
    _connOpts.set_clean_session(true);
    if (!_username.empty()) {
        _connOpts.set_user_name(_username);
        _connOpts.set_password(_password);
    }
    _client.set_callback(_callback);
}

CloudManager::~CloudManager() { 
    disconnect(); 
}
//------------------------------------------------------------------------------------
void CloudManager::connect() {
    try {
        std::cout << "[CloudManager] Connecting to " << _brokerUri << "..." << std::endl;
        _client.connect(_connOpts)->wait();
        std::cout << "[CloudManager] Connected." << std::endl;
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[CloudManager] Connection failed: " << e.what() << std::endl;
    }
}

void CloudManager::disconnect() {
    try {
        if (_client.is_connected()) {
            _client.disconnect()->wait();
            std::cout << "[CloudManager] Disconnected." << std::endl;
        }
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[CloudManager] Disconnect failed: " << e.what() << std::endl;
    }
}

bool CloudManager::isConnected() const {
    return _client.is_connected();
}

void CloudManager::publish(const std::string& topic,
                           const std::string& payload,
                           int qos,
                           bool retained) {
    if (!_client.is_connected()) {
        std::cerr << "[CloudManager] Publish failed: not connected." << std::endl;
        return;
    }
    try {
        auto msg = mqtt::make_message(topic, payload);
        msg->set_qos(qos);
        msg->set_retained(retained);
        _client.publish(msg);
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[CloudManager] Publish error: " << e.what() << std::endl;
    }
}

void CloudManager::subscribe(const std::string& topic, int qos) {
    if (!_client.is_connected()) {
        std::cerr << "[CloudManager] Subscribe failed: not connected." << std::endl;
        return;
    }
    try {
        _client.subscribe(topic, qos)->wait();
        std::cout << "[CloudManager] Subscribed to " << topic << std::endl;
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[CloudManager] Subscribe error: " << e.what() << std::endl;
    }
}

void CloudManager::setCommandHandler(CommandHandler handler) {
    _commandHandler = std::move(handler);
}

void CloudManager::onMessageArrived(mqtt::const_message_ptr msg) {
    if (_commandHandler) {
        _commandHandler(msg->get_topic(), msg->to_string());
    } else {
        std::cout << "[CloudManager] Message on " << msg->get_topic()
                  << ": " << msg->to_string() << std::endl;
    }
}

void CloudManager::onConnectionLost(const std::string& cause) {
    std::cerr << "[CloudManager] Connection lost: " << cause << std::endl;
    // Optionally: try reconnect
    try {
        _client.reconnect();
        std::cout << "[CloudManager] Reconnected." << std::endl;
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[CloudManager] Reconnect failed: " << e.what() << std::endl;
    }
}

// --- Callback adapter ---
void CloudManager::Callback::connection_lost(const std::string& cause) {
    _manager.onConnectionLost(cause);
}

void CloudManager::Callback::message_arrived(mqtt::const_message_ptr msg) {
    _manager.onMessageArrived(msg);
}