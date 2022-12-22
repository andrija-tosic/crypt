﻿namespace Cryptography.Server
{
    public interface IBlockCipher
    {
        public byte[] Encrypt(byte[] data, byte[] key);
        public byte[] Decrypt(byte[] data, byte[] key);
    }
}
