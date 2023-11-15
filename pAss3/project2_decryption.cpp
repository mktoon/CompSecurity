#include <iostream>
#include <fstream>
#include <cstring>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>

#define BUFFER_SIZE 1024

int verify_signature(const char* data_file, const char* signatureFile, const char* pubkey) {
    // Load data
    std::ifstream data_in(data_file, std::ios::binary);
    if (!data_in) {
        std::cerr << "Error: Cannot open data file" << std::endl;
        return -1;
    }
    std::string data((std::istreambuf_iterator<char>(data_in)), std::istreambuf_iterator<char>());

    // Load signature
    std::ifstream signature_in(signatureFile, std::ios::binary);
    if (!signature_in) {
        std::cerr << "Error: Cannot open signature file" << std::endl;
        return -1;
    }
    std::string signature((std::istreambuf_iterator<char>(signature_in)), std::istreambuf_iterator<char>());

    // Load public key
    FILE* pubkey_fp = fopen(pubkey, "r");
    if (!pubkey_fp) {
        std::cerr << "Error: Cannot open public key file" << std::endl;
        return -1;
    }
    RSA* rsa = PEM_read_RSA_PUBKEY(pubkey_fp, NULL, NULL, NULL);
    if (!rsa) {
        std::cerr << "Error: Cannot load public key" << std::endl;
        return -1;
    }

    // Verify signature
    int result = RSA_verify(NID_sha256, (const unsigned char*) data.c_str(), data.size(), (const unsigned char*) signature.c_str(), signature.size(), rsa);
        RSA_free(rsa);
        fclose(pubkey_fp);

        if (result != 1) {
            std::cerr << "Error: Invalid signature" << std::endl;
            return -1;
        }

        return 0;
    }

int decrypt_file(const char* input, const char* output, const char* symmetrickey) {
    // Load symmetric key
    std::ifstream key_in(symmetrickey, std::ios::binary);
    if (!key_in) {
        std::cerr << "Error: Cannot open symmetric key file" << std::endl;
        return -1;
    }
    unsigned char key[AES_BLOCK_SIZE];
    key_in.read((char*) key, AES_BLOCK_SIZE);

    // Load IV
    unsigned char iv[AES_BLOCK_SIZE];
    key_in.read((char*) iv, AES_BLOCK_SIZE);

    // Open input file
    std::ifstream in(input, std::ios::binary);
    if (!in) {
        std::cerr << "Error: Cannot open input file" << std::endl;
        return -1;
    }

    // Open output file
    std::ofstream out(output, std::ios::binary);
    if (!out) {
        std::cerr << "Error: Cannot open output file" << std::endl;
        return -1;
    }

    // Decrypt
    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);
    unsigned char input_buffer[BUFFER_SIZE];
    unsigned char output_buffer[BUFFER_SIZE];
    while (in) {
        in.read((char*)
        input_buffer, BUFFER_SIZE);
        int bytes_read = in.gcount();
        int bytes_written = 0;
        AES_cbc_encrypt(input_buffer, output_buffer, bytes_read, &aes_key, iv, AES_DECRYPT);
        out.write((char*) output_buffer, bytes_read);
    }

        // Close files
        in.close();
        out.close();
        key_in.close();

        return 0;
}

int main(int argc, char** argv) {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <signed file> <public key file> <symmetric key file>" << std::endl;
            return -1;
        }

       
        // Verify signature
        int result = verify_signature(argv[1], (std::string(argv[1]) + ".sig").c_str(), argv[2]);
        if (result != 0) {
            return result;
        }

        // Decrypt file
        result = decrypt_file(argv[1], "decrypted.txt", argv[3]);
        if (result != 0) {
            return result;
        }

        std::cout << "Decrypted file saved as decrypted.txt" << std::endl;
        return 0;
}