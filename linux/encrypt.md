# CA
CA中心为每个使用公开密钥的用户发放一个数字证书，数字证书的作用是证明证书中列出的用户合法拥有证书中列出的公开密钥。

# 数字签名
发送者：在对消息内容进行Hash运算之后，再利用私钥进行加密形成摘要的过程叫数字签名。然后将利用私钥加密后的消息内容同摘要一同发送给接受者。

接收者：使用发送者的公钥对摘要进行解密，得到一个Hash1.证明该消息确实由发送者发出。之后使用发送者的公钥在对消息内容进行解密，并自己重新进行一边Hash运算，得到Hash2。对比Hash1和Hash2，如果相同证明消息内容没有被篡改过。

# 数字证书
用户若欲获取证书，应先向CA提出申请，CA判明申请者的身份后，为之分配一个公钥，并将该公钥与其身份信息绑定，为该整体签名，签名后的整体即为证书，发还给申请者。
接收者可以从CA中心下载发送者的数字证书，获得发送者的公钥，来防止本地发送者的公钥被篡改。

# 签名与认证流程
![](https://upload.wikimedia.org/wikipedia/commons/thumb/6/6d/Digital_Signature_with_encryption%26decrypt.svg/2560px-Digital_Signature_with_encryption%26decrypt.svg.png)

# refs
http://www.youdzone.com/signature.html
http://www.ruanyifeng.com/blog/2011/08/what_is_a_digital_signature.html