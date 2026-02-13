// Libraries
#include <vector>
#include <cstring>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>

// Enrypt
void Encrypt(
    std::vector<uint8_t> &Plain_Text,
    std::vector<uint8_t> &Cipher_Text,
    std::vector<uint8_t> &Key,
    std::vector<uint8_t> &IV,
    std::vector<uint8_t> &Tag
) {

    // CTX
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // Mode Init
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);

    // IV Size Init
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV.size(), nullptr);

    // Key && IV Init
    EVP_EncryptInit_ex(ctx, nullptr, nullptr, Key.data(), IV.data());
    
    // Encrypt
    int len = 0;
    Cipher_Text.resize(Plain_Text.size());
    EVP_EncryptUpdate(ctx, Cipher_Text.data(), &len, Plain_Text.data(), Plain_Text.size());
    
    // Final
    int final_len = 0;
    EVP_EncryptFinal_ex(ctx, Cipher_Text.data() + len, &final_len);
    
    // Get Tag
    Tag.resize(16);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, Tag.size(), Tag.data());
    
    // Free CTX
    EVP_CIPHER_CTX_free(ctx);
}

// Decrypt
void Decrypt(
    std::vector<uint8_t> &Cipher_Text,
    std::vector<uint8_t> &Plain_Text,
    std::vector<uint8_t> &Key,
    std::vector<uint8_t> &IV,
    std::vector<uint8_t> &Tag
) {
    // CTX
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    // Mode Init
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr);

    // IV Size Init
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV.size(), nullptr);

    // Key && IV Init
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, Key.data(), IV.data());

    // Decrypt
    int len = 0;
    Plain_Text.resize(Cipher_Text.size());
    EVP_DecryptUpdate(ctx, Plain_Text.data(), &len, Cipher_Text.data(), Cipher_Text.size());

    // Set Tag
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, Tag.size(), (void*)Tag.data());

    // Final
    int final_len = 0;
    EVP_DecryptFinal_ex(ctx, Plain_Text.data() + len, &final_len);

    // Free
    EVP_CIPHER_CTX_free(ctx);
}

// Base64 Encode
std::string B64_Enc(const std::vector<uint8_t> &Data) {

    // Create BIO
    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;

    // Create Base64 Filter >> Memory Orianted BIO >> Push Filter Into Memory
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    // Update
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, Data.data(), Data.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer_ptr);
    
    // Free
    std::string result(buffer_ptr->data, buffer_ptr->length);
    BIO_free_all(bio);

    // Return
    return result;
}

// Base64 Decode
std::vector<uint8_t> B64_Dec(const std::string &Data) {

    // Create BIO
    BIO *bio, *b64;
    std::vector<uint8_t> Decoded_Data(Data.size());

    // Create Filter Memory BIO and PUSH
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(Data.data(), Data.size());
    bio = BIO_push(b64, bio);

    // Update
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int len = BIO_read(bio, Decoded_Data.data(), Data.size());
    Decoded_Data.resize(len);
    
    // Free
    BIO_free_all(bio);

    // Return
    return Decoded_Data;

}

// Test
int main(int argc, char *argv[]) {

    // Create Variables
    std::string Text = argv[1];
    std::vector<uint8_t> Plain_Text(Text.begin(), Text.end());
    
    std::vector<uint8_t> Key(32);
    std::vector<uint8_t> IV(12);
    RAND_bytes(Key.data(), Key.size());
    RAND_bytes(IV.data(), IV.size());
    
    // Encyrpt
    std::vector<uint8_t> Cipher_Text;
    std::vector<uint8_t> Decrypted_Text;
    std::vector<uint8_t> Tag;
    Encrypt(Plain_Text, Cipher_Text, Key, IV, Tag);
    Decrypt(Cipher_Text, Decrypted_Text, Key, IV, Tag);

    // Print
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "Key: " << B64_Enc(Key) << std::endl;
    std::cout << "IV: " << B64_Enc(IV) << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "Encrypted Data: " << B64_Enc(Cipher_Text) << std::endl;
    std::cout << "Decrypted Data: " << std::string(Decrypted_Text.begin(), Decrypted_Text.end()) << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    // Return
    return 0;

}
