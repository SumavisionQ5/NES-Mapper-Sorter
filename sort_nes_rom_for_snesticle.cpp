#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NES_HEADER_SIZE 16

// 支持的Mapper列表
const int supported_mappers[] = {0, 1, 2, 3, 4, 5, 7, 9, 11, 15, 66};
const int supported_mapper_count = sizeof(supported_mappers) / sizeof(supported_mappers[0]);

// 检查Mapper是否在支持列表中
int is_mapper_supported(int mapper) {
    for (int i = 0; i < supported_mapper_count; i++) {
        if (supported_mappers[i] == mapper) {
            return 1;
        }
    }
    return 0;
}

// 读取NES文件头并获取mapper值（支持NES2.0）
int get_mapper(const char *filename) {
    unsigned char header[NES_HEADER_SIZE];
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    if (fread(header, 1, NES_HEADER_SIZE, fp) != NES_HEADER_SIZE) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    // 检查文件头标识 "NES\x1A"
    if (memcmp(header, "NES\x1A", 4) != 0) {
        return -2;
    }

    // 检查是否为NES2.0格式（第7字节的bit2和bit3都为1）
    if ((header[7] & 0x0C) == 0x08) {
        // NES2.0格式: mapper = ((header[8] & 0x0F) << 8) | (header[7] & 0xF0) | (header[6] >> 4)
        return ((header[8] & 0x0F) << 8) | (header[7] & 0xF0) | (header[6] >> 4);
    } else {
        // iNES1.0格式: mapper = ((header[7] >> 4) << 4) | (header[6] >> 4)
        return ((header[7] >> 4) << 4) | (header[6] >> 4);
    }
}

int main() {
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH] = "D:\\nes\\*.nes";

    // 创建supported和unsupported目录
    CreateDirectoryA("D:\\nes\\supported", NULL);
    CreateDirectoryA("D:\\nes\\unsupported", NULL);

    FILE *logfp = fopen("mapperlog.txt", "w");
    if (!logfp) {
        printf("无法创建 mapperlog.txt\n");
        return 1;
    }

    hFind = FindFirstFileA(searchPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("未找到 .nes 文件！\n");
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

            // 根据Mapper是否支持选择目标目录
            const char *targetDir = is_mapper_supported(mapper) ? 
                "D:\\nes\\supported" : "D:\\nes\\unsupported";

            char targetPath[MAX_PATH];
            snprintf(targetPath, sizeof(targetPath), "%s\\%s", targetDir, ffd.cFileName);

            // 移动文件
            if (MoveFileA(fullpath, targetPath)) {
                printf("已移动到: %s\n", targetPath);
            } else {
                DWORD err = GetLastError();
                if (err == ERROR_ALREADY_EXISTS) {
                    printf("文件已存在，跳过: %s\n", targetPath);
                } else {
                    printf("移动失败 (%lu): %s\n", err, ffd.cFileName);
                }
            }
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);
    fclose(logfp);

    printf("扫描和移动完成，结果已写入 mapperlog.txt\n");
    return 0;
}
