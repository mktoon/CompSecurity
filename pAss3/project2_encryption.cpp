#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " encrypted_message pubkey.pem privkey.pem" << endl;
        return 1;
    }

    // Step 1: Decrypt the encrypted message using the third party public key
    string symm_key = argv[1];
    string pubkey = argv[2];
    string privkey = argv[3];

    RSA* rsa_pubkey = NULL;
    FILE* fp_pubkey = NULL;

    // Read the public key from the pubkey.pem
    fp_pubkey = fopen(pubkey.c_str(), "rb");
    if (fp_pubkey == NULL) {
        cerr << "Error: Failed to open public key file " << pubkey << endl;
        return 1;
    } else {
        std::cout<<"Opened public key file successfully\n";
    }

    rsa_pubkey = PEM_read_RSA_PUBKEY(fp_pubkey, NULL, NULL, NULL);
    if (rsa_pubkey == NULL) {
        cerr << "Error: Failed to read public key from file " << pubkey << endl;
        return 1;
    } else {
        cout << "Public key read successfully\n";
    }
    fclose(fp_pubkey);

    // Decrypt the encrypted message using the public key
    ifstream encrypted_message(symm_key.c_str());
    if (!encrypted_message.is_open()) {
        cerr << "Error: Failed to open encrypted message file " << symm_key << endl;
        RSA_free(rsa_pubkey);
        return 1;
    } else {
        cout << "Successfully opened encrypted message file\n";
    }

    string symmetricKey = "symmetric.txt";
    ofstream symmetric_key(symmetricKey.c_str());
    if (!symmetric_key.is_open()) {
        cerr << "Error: Failed to create symmetric key file" << symmetricKey << endl;
        RSA_free(rsa_pubkey);
        return 1;
    } else {
        cout << "Symmetric key file successfully created\n";
    }

    int rsa_size = RSA_size(rsa_pubkey);
    unsigned char* rsa_buf = new unsigned char[rsa_size];

    while (!encrypted_message.eof()) {
        encrypted_message.read(reinterpret_cast<char*>(rsa_buf), rsa_size);
        int rsa_buf_len = encrypted_message.gcount();
        unsigned char* decrypted_buf = new unsigned char[rsa_size];
        if (RSA_public_decrypt(rsa_buf_len, rsa_buf, decrypted_buf, rsa_pubkey, RSA_PKCS1_PADDING) == -1) {
            cerr << "Error: Failed to decrypt the message using the public key" << endl;
            RSA_free(rsa_pubkey);
            delete[] rsa_buf;
            encrypted_message.close();
            symmetric_key.close();
            remove(symmetricKey.c_str());
            return 1;
        }

        symmetric_key.write(reinterpret_cast<char*>(decrypted_buf), rsa_buf_len);
        delete[] decrypted_buf;
    }

    RSA_free(rsa_pubkey);
    delete[] rsa_buf;
    encrypted_message.close();
    symmetric_key.close();

    // Step 2: Encrypt a text file with the decrypted message as the symmetric key
    string originalFile = "original.txt";
    ofstream original(originalFile.c_str());
    if (!original.is_open()) {
        cerr << "Error: Failed to create original file " << originalFile << endl;
        remove(symmetricKey.c_str());
        return 1;
    }

    original << "Name Micah Too" << endl;
    original << "banner ID 800784856." << endl;

    original.close();

    string encryptedFile = "encrypted.txt";
    string symmetric_key_cmd = "openssl enc -des -in " + originalFile
                                + " -out " + encryptedFile 
                                + " -pass file:" + symmetricKey;

    int ret = system(symmetric_key_cmd.c_str());
    if ( ret != 0) {
        cerr << "Error: Failed to encrypt the original file" << endl;
        remove(symmetricKey.c_str());
        remove(originalFile.c_str());
        remove(encryptedFile.c_str());
        return 1;
    }else{
        std::cout << "Encryption Successfull" << endl;
    }

    remove(symmetricKey.c_str());
    remove(originalFile.c_str());

    // Step 3: Decrypt the encrypted file using the private key
    RSA* rsa_privkey = NULL;
    FILE* fp_privkey = NULL;

    // Read the private key from the privkey.pem file
    fp_privkey = fopen(privkey.c_str(), "rb");
    if (fp_privkey == NULL) {
        cerr << "Error: Failed to open private key file " << privkey << endl;
        return 1;
    }
    else{
        cout<<"private key file opened successfully\n";
    }

    rsa_privkey = PEM_read_RSAPrivateKey(fp_privkey, NULL, NULL, NULL);
    if (rsa_privkey == NULL) {
        cerr << "Error: Failed to read private key from file " << privkey << endl;
        return 1;
        }else{
            cout<<"private key read from file successfully\n";
        }
    fclose(fp_privkey);

    // Decrypt the encrypted file using the private key
    ifstream encrypted(encryptedFile.c_str(), ios::binary);
    if (!encrypted.is_open()) {
        cerr << "Error: Failed to open encrypted file " << encryptedFile << endl;
        RSA_free(rsa_privkey);
        return 1;
    }

    int encrypted_size = 0;
    encrypted.seekg(0, ios::end);
    encrypted_size = encrypted.tellg();
    encrypted.seekg(0, ios::beg);

    unsigned char* encrypted_buf = new unsigned char[encrypted_size];
    unsigned char* decrypted_buf = new unsigned char[encrypted_size];
    encrypted.read(reinterpret_cast<char*>(encrypted_buf), encrypted_size);
    int decrypted_size = RSA_private_decrypt(encrypted_size, encrypted_buf, decrypted_buf, rsa_privkey, RSA_PKCS1_PADDING);

    ofstream decrypted("decrypted.txt", ios::binary);
    if (!decrypted.is_open()) {
        cerr << "Error: Failed to create decrypted file decrypted.txt" << endl;
        RSA_free(rsa_privkey);
        delete[] encrypted_buf;
        delete[] decrypted_buf;
        encrypted.close();
        return 1;
    }

    decrypted.write(reinterpret_cast<char*>(decrypted_buf), decrypted_size);

    RSA_free(rsa_privkey);
    delete[] encrypted_buf;
    delete[] decrypted_buf;
    encrypted.close();
    decrypted.close();

    return 0;
}