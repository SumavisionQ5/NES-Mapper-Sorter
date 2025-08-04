#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NES_HEADER_SIZE 16

// ��ȡ iNES �ļ�ͷ����ȡ mapper ֵ
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
        return -2;
    }

    // Mapper ֵ = (flag7�ĸ�4λ << 4) + flag6�ĸ�4λ
    int mapper = ((header[7] >> 4) << 4) | (header[6] >> 4);
    return mapper;
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
            fprintf(logfp, "%s\tMapper: %d\n", ffd.cFileName, mapper);
            printf("%s -> Mapper %d\n", ffd.cFileName, mapper);

            // ---- �޸����������⣺�����ﶨ�� targetDir �� targetPath ----
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
