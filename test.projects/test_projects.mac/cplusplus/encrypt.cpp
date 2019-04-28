#include <iostream>
#include <string>

std::string encrypt(std::string msg, std::string key)
{
    // Make sure the key is at least as long as the message
    std::string tmp(key);
    while (key.size() < msg.size())
        key += tmp;
    
    // And now for the encryption part
    for (std::string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i];
    return msg;
}
std::string decrypt(std::string msg, std::string key)
{
    return encrypt(msg, key); // lol
}

int main()
{
    std::string message = encrypt("Hello world!", "monkey");
    std::cout << "Encrypted: " << message << std::endl;
    std::cout << "\nDecrypted: " << decrypt(message, "monkey") << std::endl;
}
