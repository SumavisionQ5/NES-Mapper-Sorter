#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NES_HEADER_SIZE 16

// ��ȡNES�ļ�ͷ����ȡmapperֵ��֧��NES2.0��
int get_mapper(const char *filename) {
    unsigned char header[NES_HEADER_SIZE];
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    if (fread(header, 1, NES_HEADER_SIZE, fp) != NES_HEADER_SIZE) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    // ����ļ�ͷ��ʶ "NES\x1A"
    if (memcmp(header, "NES\x1A", 4) != 0) {
        return -2;  // ��Ч��NES�ļ�ͷ
    }

    // ����Ƿ�ΪNES2.0��ʽ����7�ֽڵ�bit2��bit3��Ϊ1��
    if ((header[7] & 0x0C) == 0x08) {
        // NES2.0��ʽ: mapper = ((header[8] & 0x0F) << 8) | (header[7] & 0xF0) | (header[6] >> 4)
        return ((header[8] & 0x0F) << 8) | (header[7] & 0xF0) | (header[6] >> 4);
    } else {
        // iNES1.0��ʽ: mapper = ((header[7] >> 4) << 4) | (header[6] >> 4)
        return ((header[7] >> 4) << 4) | (header[6] >> 4);
    }
}

int main() {
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH] = "D:\\nes\\*.nes";

    FILE *logfp = fopen("mapperlog.txt", "w");
    if (!logfp) {
        printf("�޷����� mapperlog.txt\n");
        return 1;
    }

    hFind = FindFirstFileA(searchPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("δ�ҵ� .nes �ļ���\n");
        fclose(logfp);
        return 1;
    }

    do {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char fullpath[MAX_PATH];
            snprintf(fullpath, sizeof(fullpath), "D:\\nes\\%s", ffd.cFileName);

            int mapper = get_mapper(fullpath);
            
            // ������ܵĴ������
            if (mapper < 0) {
                const char* errorMsg;
                switch (mapper) {
                    case -1: errorMsg = "�ļ���ȡ����"; break;
                    case -2: errorMsg = "��Ч��NES�ļ�ͷ"; break;
                    default: errorMsg = "δ֪����";
                }
                fprintf(logfp, "%s\t����: %s\n", ffd.cFileName, errorMsg);
                printf("%s -> ����: %s\n", ffd.cFileName, errorMsg);
                continue;
            }
            
            fprintf(logfp, "%s\tMapper: %d\n", ffd.cFileName, mapper);
            printf("%s -> Mapper %d\n", ffd.cFileName, mapper);

            char targetDir[MAX_PATH];
            snprintf(targetDir, sizeof(targetDir), "D:\\nes\\Mapper-%d", mapper);

            // ����Ŀ��Ŀ¼������Ѵ��ڲ��ᱨ��
            CreateDirectoryA(targetDir, NULL);

            char targetPath[MAX_PATH];
            snprintf(targetPath, sizeof(targetPath), "%s\\%s", targetDir, ffd.cFileName);

            // �ƶ��ļ�
            if (MoveFileA(fullpath, targetPath)) {
                printf("���ƶ���: %s\n", targetPath);
            } else {
                DWORD err = GetLastError();
                if (err == ERROR_ALREADY_EXISTS) {
                    printf("�ļ��Ѵ��ڣ�����: %s\n", targetPath);
                } else {
                    printf("�ƶ�ʧ�� (%lu): %s\n", err, ffd.cFileName);
                }
            }
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);
    fclose(logfp);

    printf("ɨ����ƶ���ɣ������д�� mapperlog.txt\n");
    return 0;
}