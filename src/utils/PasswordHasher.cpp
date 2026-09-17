#include "utils/PasswordHasher.h"

#include <array>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    class SHA256
    {
    private:
        std::array<unsigned int, 8> state;
        std::array<unsigned char, 64> buffer;
        unsigned long long bitLength;
        std::size_t bufferLength;

        static unsigned int rotateRight(
            unsigned int value,
            unsigned int amount
        )
        {
            return (value >> amount) |
                   (value << (32 - amount));
        }

        static unsigned int choose(
            unsigned int x,
            unsigned int y,
            unsigned int z
        )
        {
            return (x & y) ^ (~x & z);
        }

        static unsigned int majority(
            unsigned int x,
            unsigned int y,
            unsigned int z
        )
        {
            return (x & y) ^ (x & z) ^ (y & z);
        }

        static unsigned int sigma0(unsigned int x)
        {
            return rotateRight(x, 2) ^
                   rotateRight(x, 13) ^
                   rotateRight(x, 22);
        }

        static unsigned int sigma1(unsigned int x)
        {
            return rotateRight(x, 6) ^
                   rotateRight(x, 11) ^
                   rotateRight(x, 25);
        }

        static unsigned int smallSigma0(unsigned int x)
        {
            return rotateRight(x, 7) ^
                   rotateRight(x, 18) ^
                   (x >> 3);
        }

        static unsigned int smallSigma1(unsigned int x)
        {
            return rotateRight(x, 17) ^
                   rotateRight(x, 19) ^
                   (x >> 10);
        }

        void transform()
        {
            static const unsigned int k[64] =
            {
                0x428a2f98, 0x71374491,
                0xb5c0fbcf, 0xe9b5dba5,
                0x3956c25b, 0x59f111f1,
                0x923f82a4, 0xab1c5ed5,
                0xd807aa98, 0x12835b01,
                0x243185be, 0x550c7dc3,
                0x72be5d74, 0x80deb1fe,
                0x9bdc06a7, 0xc19bf174,
                0xe49b69c1, 0xefbe4786,
                0x0fc19dc6, 0x240ca1cc,
                0x2de92c6f, 0x4a7484aa,
                0x5cb0a9dc, 0x76f988da,
                0x983e5152, 0xa831c66d,
                0xb00327c8, 0xbf597fc7,
                0xc6e00bf3, 0xd5a79147,
                0x06ca6351, 0x14292967,
                0x27b70a85, 0x2e1b2138,
                0x4d2c6dfc, 0x53380d13,
                0x650a7354, 0x766a0abb,
                0x81c2c92e, 0x92722c85,
                0xa2bfe8a1, 0xa81a664b,
                0xc24b8b70, 0xc76c51a3,
                0xd192e819, 0xd6990624,
                0xf40e3585, 0x106aa070,
                0x19a4c116, 0x1e376c08,
                0x2748774c, 0x34b0bcb5,
                0x391c0cb3, 0x4ed8aa4a,
                0x5b9cca4f, 0x682e6ff3,
                0x748f82ee, 0x78a5636f,
                0x84c87814, 0x8cc70208,
                0x90befffa, 0xa4506ceb,
                0xbef9a3f7, 0xc67178f2
            };

            std::array<unsigned int, 64> w{};

            for (int i = 0; i < 16; ++i)
            {
                w[i] =
                    (static_cast<unsigned int>(buffer[i * 4]) << 24) |
                    (static_cast<unsigned int>(buffer[i * 4 + 1]) << 16) |
                    (static_cast<unsigned int>(buffer[i * 4 + 2]) << 8) |
                    static_cast<unsigned int>(buffer[i * 4 + 3]);
            }

            for (int i = 16; i < 64; ++i)
            {
                w[i] =
                    smallSigma1(w[i - 2]) +
                    w[i - 7] +
                    smallSigma0(w[i - 15]) +
                    w[i - 16];
            }

            unsigned int a = state[0];
            unsigned int b = state[1];
            unsigned int c = state[2];
            unsigned int d = state[3];
            unsigned int e = state[4];
            unsigned int f = state[5];
            unsigned int g = state[6];
            unsigned int h = state[7];

            for (int i = 0; i < 64; ++i)
            {
                unsigned int temp1 =
                    h +
                    sigma1(e) +
                    choose(e, f, g) +
                    k[i] +
                    w[i];

                unsigned int temp2 =
                    sigma0(a) +
                    majority(a, b, c);

                h = g;
                g = f;
                f = e;
                e = d + temp1;
                d = c;
                c = b;
                b = a;
                a = temp1 + temp2;
            }

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
            state[4] += e;
            state[5] += f;
            state[6] += g;
            state[7] += h;
        }

    public:
        SHA256()
            : state{
                  0x6a09e667,
                  0xbb67ae85,
                  0x3c6ef372,
                  0xa54ff53a,
                  0x510e527f,
                  0x9b05688c,
                  0x1f83d9ab,
                  0x5be0cd19
              },
              buffer{},
              bitLength(0),
              bufferLength(0)
        {
        }

        void update(const std::string& input)
        {
            for (unsigned char byte : input)
            {
                buffer[bufferLength++] = byte;
                bitLength += 8;

                if (bufferLength == 64)
                {
                    transform();
                    bufferLength = 0;
                }
            }
        }

        std::string finalize()
        {
            const unsigned long long originalBitLength =
                bitLength;

            buffer[bufferLength++] = 0x80;

            if (bufferLength > 56)
            {
                while (bufferLength < 64)
                {
                    buffer[bufferLength++] = 0;
                }

                transform();
                bufferLength = 0;
            }

            while (bufferLength < 56)
            {
                buffer[bufferLength++] = 0;
            }

            for (int i = 7; i >= 0; --i)
            {
                buffer[bufferLength++] =
                    static_cast<unsigned char>(
                        (originalBitLength >> (i * 8)) & 0xff
                    );
            }

            transform();

            std::ostringstream output;

            output << std::hex
                   << std::setfill('0');

            for (unsigned int value : state)
            {
                output << std::setw(8)
                       << value;
            }

            return output.str();
        }
    };

    std::string generateSalt()
    {
        std::random_device randomDevice;

        std::ostringstream salt;

        salt << std::hex
             << std::setfill('0');

        for (int i = 0; i < 16; ++i)
        {
            const unsigned int value =
                randomDevice();

            salt << std::setw(8)
                 << value;
        }

        return salt.str();
    }

    std::string sha256(
        const std::string& input
    )
    {
        SHA256 hasher;

        hasher.update(input);

        return hasher.finalize();
    }
}

std::string PasswordHasher::hashPassword(
    const std::string& password
)
{
    if (password.empty())
    {
        throw std::invalid_argument(
            "Password cannot be empty."
        );
    }

    const std::string salt = generateSalt();

    const std::string hash =
        sha256(salt + password);

    return salt + ":" + hash;
}

bool PasswordHasher::verifyPassword(
    const std::string& password,
    const std::string& storedHash
)
{
    if (password.empty())
    {
        throw std::invalid_argument(
            "Password cannot be empty."
        );
    }

    const std::size_t separator =
        storedHash.find(':');

    if (separator == std::string::npos)
    {
        return false;
    }

    const std::string salt =
        storedHash.substr(0, separator);

    const std::string expectedHash =
        storedHash.substr(separator + 1);

    if (salt.empty() || expectedHash.empty())
    {
        return false;
    }

    const std::string actualHash =
        sha256(salt + password);

    return actualHash == expectedHash;
}