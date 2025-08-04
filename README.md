# NES-Mapper-Sorter

一个小工具，用于**扫描并按 Mapper 值整理 NES (iNES) ROM 文件**。  
它会读取 `iNES` 文件头中的 Mapper 编号，将同一 Mapper 的游戏移动到对应文件夹下，并生成 `mapperlog.txt`。

---

## 功能特点

- 自动扫描指定目录下的所有 `.nes` 文件
- 解析 iNES 文件头，获取 Mapper 编号
- 按 Mapper 编号创建对应文件夹：`Mapper-0`, `Mapper-1` ...
- 自动将 NES ROM 文件移动到对应目录
- 生成一个 `mapperlog.txt`，记录所有文件及其 Mapper 值

---

## 用法

1. 下载并编译本项目源码（支持 MinGW / MSVC）。
2. 确保待整理的 ROM 文件放在某个文件夹，例如：

```

D:\nes
├── 10-Yard Fight (USA, Europe).nes
├── Super Mario Bros.nes
└── ...

````

3. 修改源码中的目标路径：
```c
char searchPath[MAX_PATH] = "D:\\nes\\*.nes";
````

或后续改为命令行参数方式。

4. 运行程序，它会自动：

   * 读取 Mapper 编号
   * 创建对应目录 `D:\nes\Mapper-XX`
   * 移动文件到对应目录
   * 在当前目录生成 `mapperlog.txt`

5. 整理结果示例：

   ```
   D:\nes\
      ├── Mapper-0\
      │     └── 10-Yard Fight (USA, Europe).nes
      ├── Mapper-1\
      │     └── The Legend of Zelda (USA).nes
      └── mapperlog.txt
   ```

---

## 输出日志

程序会在根目录生成一个 `mapperlog.txt`，内容格式如下：

```
10-Yard Fight (USA, Europe).nes    Mapper: 0
The Legend of Zelda (USA).nes      Mapper: 1
...
```

---

## 编译方法

### Windows (MinGW)

```bash
g++ -o NES-Mapper-Sorter.exe main.cpp
```

### Windows (MSVC)

在 Visual Studio 打开项目，直接编译即可。

---


## 许可证

MIT License

```

---


