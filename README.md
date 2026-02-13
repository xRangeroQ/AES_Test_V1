# AES Test V1 🔐
- A lightweight, high-performance C++ tool for AES-256 encryption using OpenSSL. This project is compiled as a static binary, meaning it has zero external dependencies and runs out-of-the-box on Windows.

## ✨ Key Features
- Static Build: No need for OpenSSL .dll files; all dependencies are embedded into the .exe.
- Fast Execution: Optimized C++ code for rapid encryption/decryption tasks.
- Secure: Leverages industry-standard OpenSSL crypto libraries.

# General Usage
```bash
./AES_Test.exe "Your secret message here"
```

## 🛠️ Build Information (For Developers)
- If you wish to compile this project manually on Windows using MinGW (g++), ensure you link the following system libraries to satisfy OpenSSL's internal requirements:

```bash
g++ main.cpp -o AES_Test.exe -lssl -lcrypto -lws2_32 -lgdi32 -lcrypt32
```

## IMPORTANT
- Note on Static Linking: The use of -lws2_32 and -lcrypt32 is mandatory for Windows builds as OpenSSL's static library references these Windows API components for entropy and networking.
