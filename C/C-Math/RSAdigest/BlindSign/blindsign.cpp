#include "inc/randpool.h"
#include "inc/rsa.h"
#include "inc/hex.h"
#include "inc/files.h"

#include <iostream>

using namespace std;

//Crypto++ SDK
using namespace CryptoPP;
//������������cryptlib.lib
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
    cout<<"          ����RSA��äǩ����ʾ����                      "<<endl;
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
 
    //�����ز�����Ϣ
    cout<<"ѡȡ��2������:"<<"p:"<<hex<<privKey.GetPrime1()<<"\tq:"<<hex<<privKey.GetPrime2()<<endl;
    cout<<"��Կ(e,n):"<<"("<<hex<<e<<","<<hex<<n<<")"<<endl;
    cout<<"˽Կ(d,n):"<<"("<<hex<<d<<","<<hex<<n<<")\n\n"<<endl;
    /////////////////////////////////////////////////////////
    string message, recovered;
    Integer m, fm,c, r, k(128), m_, s_, s;
     
    message = "NCEPU Sci&Teque";
    cout << "messageԭ��: " << message << endl;
     
    // Treat the message as a big endian array
    m = Integer((const byte *)message.data(), message.size());
    cout << "ä������Ϣm: " << hex << m << endl;
 
    // Encrypt and Blind
    //c = pubKey.ApplyFunction(m);
    //cout << "c: " << hex << c << endl;
    fm=m;
    m_=a_exp_b_mod_c(k,e,n);
    m_=m_*fm%n;
    cout<<"���ܺ��ä����Ϣm_:"<<hex<<m_<<endl<<endl<<endl;
 
    // Decrypt
    //r = privKey.CalculateInverse(prng, c);
    //cout << "r: " << hex << r << endl;
 
    //Sign
    s_=a_exp_b_mod_c(m_,d,n);
    cout<<"ǩ����ļ�����Ϣs_:"<<hex<<s_<<endl;
 
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
    //��֤�Ƿ�����������
    s=a_exp_b_mod_c(fm,d,n);
    cout<<"s:"<<hex<<s<<endl;
    cout<<"��ä���ǩ��s:"<<endl;
    cout<<"\n��ʼ��֤ǩ���ĺϷ���.."<<endl;
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
        cout<<"\n��ʼ������Ϣ.."<<endl;
        size_t req=fm.MinEncodedSize();
        recovered.resize(req);
        fm.Encode((byte*)recovered.data(),recovered.size());
        cout<<"���ܺ����Ϣrecovered:"<<recovered<<endl;
        cout<<"���ܳɹ���"<<endl;
    }
 
    system("pause");
 
    return 0;
}

//------------------------
// ��������
//------------------------
//����RSA��Կ�Եĺ�������
void GenerateRSAKey(unsigned int keyLength, const char *privFilename, 
	const char *pubFilename, const char *seed);
//RSA���ܺ���
string RSAEncryptString(const char *pubFilename, 
	const char *seed, const char *message);
//RSA���ܺ���
string RSADecryptString(const char *privFilename, const char *ciphertext);
//ȫ���������
RandomPool & GlobalRNG();

//�ļ���ä��
//�ļ���ǩ��
//ȥä��

//------------------------
// ������
//------------------------

int RsaDemo()
{
	char priKey[128] = {0};
	char pubKey[128] = {0};
	char seed[1024] = {0};
	// ���� RSA ��Կ��
	strcpy(priKey, "pri"); // ���ɵ�˽Կ�ļ�
	strcpy(pubKey, "pub"); // ���ɵĹ�Կ�ļ�
	strcpy(seed, "seed"); //���������ļ�
	GenerateRSAKey(1024, priKey, pubKey, seed);

	// RSA ����
	char message[1024] = {0};
	strcpy(message, "LENOVO PRODUCT");

	cout<<"Origin Text��ԭ�ģ�:\t"<< message <<endl<<endl;

	string encryptedText = RSAEncryptString(pubKey, seed, message); // RSA ����

	cout<<"Encrypted Text�����ģ�:\t"<<encryptedText<<endl<<endl;

	// RSA ����
	string decryptedText = RSADecryptString(priKey, encryptedText.c_str()); // RSA ����
	
	cout<<"Decrypted Text�����ܺ��ı���:\t"<<decryptedText<<endl<<endl;

	system("pause"); 

	return 0;
}

//------------------------
// ����RSA��Կ��
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
// RSA����
//------------------------
string RSAEncryptString(const char *pubFilename, const char *seed, const char *message)
{
	FileSource pubFile(pubFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.Put((byte *)seed, strlen(seed));

	//1.����һ���ַ�������ű���Ľ��
	string result;

	//2.����һ�����������󣬲�ͨ�� StringSink ���������
	StringSource(message, true, 
		new PK_EncryptorFilter (randPool, pub, 
			new HexEncoder(new StringSink(result))
		)
	);

	return result;
}

//------------------------
// RSA����
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
// ����ȫ�ֵ��������
//------------------------
RandomPool& GlobalRNG()
{
	static RandomPool randomPool;
	return randomPool;
}