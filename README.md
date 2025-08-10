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

1. Windows系统下，下载本项目编译好的EXE文件。
2. 确保待整理的后缀为nes的ROM文件放在D:\nes\下，例如：

```

D:\nes
├── 10-Yard Fight (USA, Europe).nes
├── Super Mario Bros.nes
└── ...

````

3.  运行程序，它会自动：

   * 读取 Mapper 编号
   * 创建对应目录 `D:\nes\Mapper-XX`
   * 移动文件到对应目录
   * 在当前目录生成 `mapperlog.txt`

4. 整理结果示例：

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

程序会在EXE所在的目录生成一个 `mapperlog.txt`，内容格式如下：

```
10-Yard Fight (USA, Europe).nes    Mapper: 0
The Legend of Zelda (USA).nes      Mapper: 1
...

### 可以自己修改源代码的路径重新编译EXE。


