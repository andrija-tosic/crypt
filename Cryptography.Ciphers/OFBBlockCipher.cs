﻿namespace Cryptography.Ciphers;
public class OFBBlockCipher : IDisposable
{
    private readonly IBlockCipher blockCipher;

    private byte[] outputFeedback;

    private ByteBlockSplitter blockSplitter;

    public OFBBlockCipher(IBlockCipher blockCipher, byte[] IV)
    {
        this.blockCipher = blockCipher;

        this.outputFeedback = new byte[IV.Length];
        Array.Copy(IV, this.outputFeedback, IV.Length);

        this.blockSplitter = new ByteBlockSplitter(IV.Length);
    }

    public IEnumerable<Memory<byte>> Encrypt(byte[] plaintext)
    {
        if (plaintext.Length == 0)
        {
            yield return plaintext;
        }

        foreach (Memory<byte> block in this.blockSplitter.SplitToBlocks(plaintext))
        {
            this.outputFeedback = this.blockCipher.EncryptBlock(this.outputFeedback, this.blockCipher.Key);

            for (int i = 0; i < this.outputFeedback.Length; i++)
            {
                block.Span[i] ^= this.outputFeedback[i];
            }

            yield return block;
        }
    }

    public byte[] Finish()
    {
        byte[] leftOverBytes = this.blockSplitter.Flush();

        this.outputFeedback = this.blockCipher.EncryptBlock(this.outputFeedback, this.blockCipher.Key);

        for (int i = 0; i < leftOverBytes.Length; i++)
        {
            leftOverBytes[i] ^= this.outputFeedback[i];
        }

        return leftOverBytes;
    }

    public IEnumerable<Memory<byte>> Decrypt(byte[] plaintext)
    {
        return this.Encrypt(plaintext);
    }

    public void Dispose()
    {
        GC.SuppressFinalize(this);
    }
}