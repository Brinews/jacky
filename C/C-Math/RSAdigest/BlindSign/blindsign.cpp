#include "inc/randpool.h"
#include "inc/rsa.h"
#include "inc/hex.h"
#include "inc/files.h"

#include <iostream>

using namespace std;

//Crypto++ SDK
using namespace CryptoPP;
//编译链接生成cryptlib.lib
#pragma comment(lib, "cryptlib.lib")

#include <iostream>
using std::cout;
using std::endl;
 
#include <cstdlib>
 
#include <iomanip>
using std::hex;
 
#include <string>
using std::string;
 
using CryptoPP::RSA;
 
#include "inc/integer.h"
using CryptoPP::Integer;
 
#include "inc/osrng.h"
using CryptoPP::AutoSeededRandomPool;
 
int main(int argc, char** argv)
{
    cout<<"======================================================="<<endl;
    cout<<"          基于RSA的盲签名演示程序                      "<<endl;
    cout<<"======================================================="<<endl;
 
    AutoSeededRandomPool prng;
 
    /////////////////////////////////////////////////////////
    /*
     RSA::PrivateKey privKey;
     privKey.GenerateRandomWithKeySize(prng, 128);
     RSA::PublicKey pubKey(privKey);
 
     cout << "modulus: " << hex << privKey.GetModulus() << endl;
     cout << "private exp: " << hex << privKey.GetPrivateExponent() << endl;
     cout << "public exp: " << hex << privKey.GetPublicExponent() << endl;
     cout << endl;
     */
    /////////////////////////////////////////////////////////
     
    Integer n("0xbeaadb3d839f3b5f"), e("0x11"), d("0x21a5ae37b9959db9");
 
    RSA::PrivateKey privKey;
    privKey.Initialize(n, e, d);
 
    RSA::PublicKey pubKey;
    pubKey.Initialize(n, e);
 
    //输出相关参数信息
    cout<<"选取的2个素数:"<<"p:"<<hex<<privKey.GetPrime1()<<"\tq:"<<hex<<privKey.GetPrime2()<<endl;
    cout<<"公钥(e,n):"<<"("<<hex<<e<<","<<hex<<n<<")"<<endl;
    cout<<"私钥(d,n):"<<"("<<hex<<d<<","<<hex<<n<<")\n\n"<<endl;
    /////////////////////////////////////////////////////////
    string message, recovered;
    Integer m, fm,c, r, k(128), m_, s_, s;
     
    message = "NCEPU Sci&Teque";
    cout << "message原文: " << message << endl;
     
    // Treat the message as a big endian array
    m = Integer((const byte *)message.data(), message.size());
    cout << "盲化的消息m: " << hex << m << endl;
 
    // Encrypt and Blind
    //c = pubKey.ApplyFunction(m);
    //cout << "c: " << hex << c << endl;
    fm=m;
    m_=a_exp_b_mod_c(k,e,n);
    m_=m_*fm%n;
    cout<<"加密后的盲化消息m_:"<<hex<<m_<<endl<<endl<<endl;
 
    // Decrypt
    //r = privKey.CalculateInverse(prng, c);
    //cout << "r: " << hex << r << endl;
 
    //Sign
    s_=a_exp_b_mod_c(m_,d,n);
    cout<<"签名后的加密消息s_:"<<hex<<s_<<endl;
 
    //unblind
    for(int nani=1;;nani++)
    {
        s_+=n;
        if(s_%k==0)
        {
            s=s_/k%n;
            cout<<"nani:"<<nani<<endl;//29
            cout<<"s:"<<hex<<s<<endl;
            break;
        }
    }
    //验证是否与上面的相等
    s=a_exp_b_mod_c(fm,d,n);
    cout<<"s:"<<hex<<s<<endl;
    cout<<"解盲后的签名s:"<<endl;
    cout<<"\n开始验证签名的合法性.."<<endl;
    bool bVerify=true;
    if(fm%n == a_exp_b_mod_c(s,e,n))
    {
        bVerify=true;
        cout<<"Verify Success!\n"<<endl;
    }
    else
    {
        bVerify=false;
        cout<<"Vefify Failed.\n"<<endl;
    }
     
    // Round trip the message
 
    if(bVerify)
    {
        cout<<"\n开始解密消息.."<<endl;
        size_t req=fm.MinEncodedSize();
        recovered.resize(req);
        fm.Encode((byte*)recovered.data(),recovered.size());
        cout<<"解密后的消息recovered:"<<recovered<<endl;
        cout<<"解密成功！"<<endl;
    }
 
    system("pause");
 
    return 0;
}

//------------------------
// 函数声明
//------------------------
//生成RSA密钥对的函数声明
void GenerateRSAKey(unsigned int keyLength, const char *privFilename, 
	const char *pubFilename, const char *seed);
//RSA加密函数
string RSAEncryptString(const char *pubFilename, 
	const char *seed, const char *message);
//RSA解密函数
string RSADecryptString(const char *privFilename, const char *ciphertext);
//全局随机数池
RandomPool & GlobalRNG();

//文件的盲化
//文件的签名
//去盲化

//------------------------
// 主程序
//------------------------

int RsaDemo()
{
	char priKey[128] = {0};
	char pubKey[128] = {0};
	char seed[1024] = {0};
	// 生成 RSA 密钥对
	strcpy(priKey, "pri"); // 生成的私钥文件
	strcpy(pubKey, "pub"); // 生成的公钥文件
	strcpy(seed, "seed"); //生成种子文件
	GenerateRSAKey(1024, priKey, pubKey, seed);

	// RSA 加密
	char message[1024] = {0};
	strcpy(message, "LENOVO PRODUCT");

	cout<<"Origin Text（原文）:\t"<< message <<endl<<endl;

	string encryptedText = RSAEncryptString(pubKey, seed, message); // RSA 加密

	cout<<"Encrypted Text（密文）:\t"<<encryptedText<<endl<<endl;

	// RSA 解密
	string decryptedText = RSADecryptString(priKey, encryptedText.c_str()); // RSA 解密
	
	cout<<"Decrypted Text（解密后文本）:\t"<<decryptedText<<endl<<endl;

	system("pause"); 

	return 0;
}

//------------------------
// 生成RSA密钥对
//------------------------
void GenerateRSAKey(unsigned int keyLength, const char *privFilename,
	const char *pubFilename, const char *seed)
{
	RandomPool randPool;
	randPool.Put((byte *)seed, strlen(seed));

	RSAES_OAEP_SHA_Decryptor priv(randPool, keyLength);
	HexEncoder privFile(new FileSink(privFilename));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pubFilename));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();
}

//------------------------
// RSA加密
//------------------------
string RSAEncryptString(const char *pubFilename, const char *seed, const char *message)
{
	FileSource pubFile(pubFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.Put((byte *)seed, strlen(seed));

	//1.声明一个字符串来存放编码的结果
	string result;

	//2.声明一个编码器对象，并通过 StringSink 类关联两者
	StringSource(message, true, 
		new PK_EncryptorFilter (randPool, pub, 
			new HexEncoder(new StringSink(result))
		)
	);

	return result;
}

//------------------------
// RSA解密
//------------------------
string RSADecryptString(const char *privFilename, const char *ciphertext)
{
	FileSource privFile(privFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	string result;
	StringSource(ciphertext, true, 
		new HexDecoder(new PK_DecryptorFilter(GlobalRNG(), priv, 
			new StringSink(result))
		)
	);

	return result;
}

//------------------------
// 定义全局的随机数池
//------------------------
RandomPool& GlobalRNG()
{
	static RandomPool randomPool;
	return randomPool;
}