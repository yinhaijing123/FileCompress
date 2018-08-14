#pragma once
#include"HuffmanTree.h"
#include<string>

typedef long long LongType;
#pragma once
#include"HuffmanTree.h"
#include<string>

typedef long long LongType;

struct CharInfo
{
    char _ch;
    LongType _count;                  //根节点表示所有次数之和，为避免越界
    string _code;                     //编码长度不确定，故可用string类型

    bool operator!=(const CharInfo& info)
    {
        return _count != info._count;
    }

    CharInfo operator+(const CharInfo& info)
    {
        CharInfo ret;
        ret._count= _count + info._count;
        return ret;
    }

    bool operator>(const CharInfo& info)
    {
        return _count > info._count;
    }
};

class FileCompress
{
    typedef HuffmanTreeNode<CharInfo> Node;
    struct TmpInfo
    {
        char _ch;
        LongType _count;
    };

public:

    FileCompress();
    void Compress(const char *file);
    void Uncompress(const char *file);
    void GenerateHuffmanCode(Node *root, string code);

protected:
    CharInfo _infos[256];
};

FileCompress::FileCompress()          
{
    for (int i = 0; i < 256; ++i)
    {
        _infos[i]._ch = (char)i;
        _infos[i]._count = 0;
        _infos[i]._code;
    }
    
}

void FileCompress::Compress(const char *file)
{

    
    FILE *fout = fopen(file, "r");
    assert(fout);

	//1.统计字符次数
    char ch = fgetc(fout);
    while (ch != EOF)
    {
        _infos[(unsigned char)ch]._count++;
        ch = fgetc(fout);
    }

    //2.生成Huffman树,进而生成Huffman code
    CharInfo invalid;
    invalid._count = 0;
    HuffmanTree<CharInfo> tree(_infos, 256, invalid);


    string compressfile = file;
    compressfile = compressfile + ".huffman";
    FILE *fin = fopen(compressfile.c_str(), "w");

    string code;
    GenerateHuffmanCode(tree.GetRoot(), code);
    
    //压缩之前先把文件信息保存，以备解压使用
    TmpInfo info;
    for (int i = 0; i < 256; i++)
    {
        if (_infos[i]._count > 0)
        {
            
            info._ch = _infos[i]._ch;
            info._count = _infos[i]._count;
            fwrite(&info, sizeof(info), 1, fin);
       }
    }
    info._count = -1;
    fwrite(&info, sizeof(info), 1, fin);
    
	//3.压缩
    fseek(fout, 0, SEEK_SET);
    ch = fgetc(fout);

    int pos = 0;
    int value = 0;
    int flag = 1;
    while (ch != EOF)
    {
        string& code = _infos[(unsigned char)ch]._code; 
        for (size_t i = 0; i < code.size(); ++i)           //处理字符
        {
            if (code[i] == '1')                          
            {
                value |= (1 << pos);
            }
            else if (code[i] == '0')                       
            {
                value &= (~(1<< pos));
            }
            else
            {
                assert(false);
            }

            ++pos;

            if (pos == 8)                                  //表示已经处理了一个字符，可以写入压缩文件中
            {
                fputc(value,fin);
                pos = 0;
                value = 0;
            }
        }

        ch = fgetc(fout);                        
    }

	//最后一个value未写够一个字节
    if (pos > 0)                              
    {
        fputc(value, fin);
    }
    fclose(fout);
    fclose(fin);
}

void FileCompress::Uncompress(const char *file)
{
    string uncompress = file;
    size_t pos = uncompress.rfind('.');
    uncompress.erase(pos, uncompress.npos);
    pos = uncompress.rfind('.');
	//为了和压缩前的文件名进行区分，解压后的文件名为在原文件名后多加了一个字符'1'   input.txt --> input1.txt
    uncompress.insert(pos, 1, '1');

    FILE *fout = fopen(file, "r");
    FILE* fin = fopen(uncompress.c_str(), "w");

	TmpInfo info;
    fread(&info, sizeof(info), 1, fout);
    while(info._count!=-1)
    {
        _infos[unsigned char(info._ch)]._ch = info._ch;
        _infos[unsigned char(info._ch)]._count = info._count;
        fread(&info, sizeof(info), 1, fout);
    }

    //1.重建Huffman tree
    CharInfo invalid;
    invalid._count = 0;
    HuffmanTree<CharInfo> tree(_infos, 256, invalid);
	
    if (tree.GetRoot() == NULL)
    {
        printf("建树失败\n");
        return;
    }
	
	//2.读取一个字节，根据其Huffman code还原原字符
    Node *root = tree.GetRoot();
    Node *cur = root;
    LongType n = root->_w._count;

    char ch=fgetc(fout);
    while (ch != EOF)
    {
        for (int i = 0; i < 8; ++i)
        {
            if ((ch&(1 << i)) == 0)
            {
                cur = cur->_left;
            }
            else
            {
                cur = cur->_right;
            }
            if (cur->_left == NULL && cur->_right == NULL)
            {
                fputc(cur->_w._ch, fin);
                cur = root;
                if ((--n)==0)
                {
                    break;
                } 
            }
        }
        ch=fgetc(fout);
    }
    fclose(fin);
    fclose(fout);
}

void FileCompress::GenerateHuffmanCode(Node *root, string code)
{
    if (root == NULL)
    {
        return;
    }
    if (root->_left == NULL && root->_right == NULL)
    {
        _infos[(unsigned char)root->_w._ch]._code = code;
        return;
    }

    GenerateHuffmanCode(root->_left, code + '0');
    GenerateHuffmanCode(root->_right, code + '1');
}

/////////////////////////////////////////////////////////////////////////////////

#define TESTFILECOMPRESS
#ifdef TESTFILECOMPRESS
void TestFileCompress()
{
    FileCompress fc1;
    fc1.Compress("../input.txt");

    FileCompress fc2;
    fc2.Uncompress("../input.txt.huffman");
}
#endif