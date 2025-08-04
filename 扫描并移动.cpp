#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NES_HEADER_SIZE 16

// 读取 iNES 文件头并获取 mapper 值
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

    // Mapper 值 = (flag7的高4位 << 4) + flag6的高4位
    int mapper = ((header[7] >> 4) << 4) | (header[6] >> 4);
    return mapper;
}

int main() {
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH] = "D:\\nes\\*.nes";

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

            // ---- 修复作用域问题：在这里定义 targetDir 和 targetPath ----
            char targetDir[MAX_PATH];
            snprintf(targetDir, sizeof(targetDir), "D:\\nes\\Mapper-%d", mapper);

            // 创建目标目录（如果已存在不会报错）
            CreateDirectoryA(targetDir, NULL);

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
