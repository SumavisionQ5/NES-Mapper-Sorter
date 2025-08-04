**NES Mappers Supported by SNESticle**

By disassembling the PS2 version binary of SNESticle using IDA Pro, we obtained the following function:

```c
const char *__fastcall sub_140AD8(int a1)
{
  const char *v1; // $v1
  int v2; // $v0

  v1 = "MMC5";
  if ( a1 != 5 )
  {
    if ( a1 >= 6 )
    {
      v1 = "ColorDreams";
      if ( a1 == 11 )
        return v1;
      if ( a1 >= 12 )
      {
        v1 = "GNROM";
        if ( a1 == 66 )
          return v1;
        if ( a1 < 67 )
        {
          v2 = 15;
          v1 = "100-in-1";
        }
        else
        {
          v2 = 255;
          v1 = "Famicom Disk System";
        }
      }
      else
      {
        v1 = "AOROM";
        if ( a1 == 7 )
          return v1;
        v2 = 9;
        v1 = "MMC2";
      }
    }
    else
    {
      v1 = "UNROM";
      if ( a1 == 2 )
        return v1;
      if ( a1 >= 3 )
      {
        v1 = "CNROM";
        if ( a1 == 3 )
          return v1;
        v2 = 4;
        v1 = "MMC3";
      }
      else
      {
        v1 = "None";
        if ( !a1 )
          return v1;
        v2 = 1;
        v1 = "MMC1";
      }
    }
    if ( a1 != v2 )
      return "<unknown>";
  }
  return v1;
}
```

After analyzing this with ChatGPT, we get the following result:

---

### ✅ Summary: This function corresponds to the following mapper name logic:

| Mapper ID | Name                |
| --------- | ------------------- |
| 0         | None                |
| 1         | MMC1                |
| 2         | UNROM               |
| 3         | CNROM               |
| 4         | MMC3                |
| 5         | MMC5                |
| 7         | AOROM               |
| 9         | MMC2                |
| 11        | ColorDreams         |
| 15        | 100-in-1            |
| 66        | GNROM               |
| 255       | Famicom Disk System |
| Others    | <unknown>           |

---

### 🔎 Additional Notes:

* This style of implementation might be the result of compiler optimization (e.g., transforming a switch-case or table lookup into a decision tree).
* If you want the function to be clearer or easier to extend, you can rewrite it as a `struct` table or a switch-case.

---

### 🛠️ Example: Rewritten as a Lookup Table (more suitable for future patching)

```c
typedef struct {
    int mapper;
    const char* name;
} MapperEntry;

static const MapperEntry mapper_table[] = {
    {0, "None"},
    {1, "MMC1"},
    {2, "UNROM"},
    {3, "CNROM"},
    {4, "MMC3"},
    {5, "MMC5"},
    {7, "AOROM"},
    {9, "MMC2"},
    {11, "ColorDreams"},
    {15, "100-in-1"},
    {66, "GNROM"},
    {255, "Famicom Disk System"},
};

const char* get_mapper_name(int mapper)
{
    for (int i = 0; i < sizeof(mapper_table)/sizeof(mapper_table[0]); i++) {
        if (mapper_table[i].mapper == mapper)
            return mapper_table[i].name;
    }
    return "<unknown>";
}
```

This version is more readable and easier to maintain.

---

After testing, it was found that although SNESticle supports the above mappers, its compatibility is mediocre. Aside from mappers 0, 1, 2, 3, 4, 5, and 15, others are technically supported but in practice are poorly compatible. Even for the relatively compatible mapper 4, it fails to run games like *Teenage Mutant Ninja Turtles III*.

By disassembling other parts of the code using IDA Pro, it was also discovered that although SNESticle supports FDS (requires placing `disksys.rom` in the same folder as the FDS files), its support is very poor—most FDS games fail to run, with only a handful actually working.

=====================================================================================

SNESticle支持的NES的Mapper

通过用IDA Pro反编译SNESticle的PS2版二进制文件，得到以下函数：
const char *__fastcall sub_140AD8(int a1)
{
  const char *v1; // $v1
  int v2; // $v0

  v1 = "MMC5";
  if ( a1 != 5 )
  {
    if ( a1 >= 6 )
    {
      v1 = "ColorDreams";
      if ( a1 == 11 )
        return v1;
      if ( a1 >= 12 )
      {
        v1 = "GNROM";
        if ( a1 == 66 )
          return v1;
        if ( a1 < 67 )
        {
          v2 = 15;
          v1 = "100-in-1";
        }
        else
        {
          v2 = 255;
          v1 = "Famicom Disk System";
        }
      }
      else
      {
        v1 = "AOROM";
        if ( a1 == 7 )
          return v1;
        v2 = 9;
        v1 = "MMC2";
      }
    }
    else
    {
      v1 = "UNROM";
      if ( a1 == 2 )
        return v1;
      if ( a1 >= 3 )
      {
        v1 = "CNROM";
        if ( a1 == 3 )
          return v1;
        v2 = 4;
        v1 = "MMC3";
      }
      else
      {
        v1 = "None";
        if ( !a1 )
          return v1;
        v2 = 1;
        v1 = "MMC1";
      }
    }
    if ( a1 != v2 )
      return "<unknown>";
  }
  return v1;
}

通过扔给chatGPT分析，得到以下回答：

### ✅ 总结：这段代码对应的 mapper 名称逻辑如下：

| Mapper ID | 名称                  |
| --------- | ------------------- |
| 0         | None                |
| 1         | MMC1                |
| 2         | UNROM               |
| 3         | CNROM               |
| 4         | MMC3                |
| 5         | MMC5                |
| 7         | AOROM               |
| 9         | MMC2                |
| 11        | ColorDreams         |
| 15        | 100-in-1            |
| 66        | GNROM               |
| 255       | Famicom Disk System |
| 其它未匹配     | `<unknown>`         |

---

### 🔎 补充说明：

* 这种写法可能是编译器优化后的结果（比如从一个 `switch-case` 或表查找转成了逻辑判断树）。
* 若你想让这个函数更清晰或便于扩展，可以改写为一个 `struct` 表或 `switch-case`。

---

### 🛠️ 示例改写为查找表（适合未来你要 patch）：

typedef struct {
    int mapper;
    const char* name;
} MapperEntry;

static const MapperEntry mapper_table[] = {
    {0, "None"},
    {1, "MMC1"},
    {2, "UNROM"},
    {3, "CNROM"},
    {4, "MMC3"},
    {5, "MMC5"},
    {7, "AOROM"},
    {9, "MMC2"},
    {11, "ColorDreams"},
    {15, "100-in-1"},
    {66, "GNROM"},
    {255, "Famicom Disk System"},
};

const char* get_mapper_name(int mapper)
{
    for (int i = 0; i < sizeof(mapper_table)/sizeof(mapper_table[0]); i++) {
        if (mapper_table[i].mapper == mapper)
            return mapper_table[i].name;
    }
    return "<unknown>";
}

这样更易读、易维护。

经过测试，发现SNESticle虽然支持上述Mapper，但是兼容性一般，除了0、1、2、3、4、5、15以外，其他虽然属于支持的Mapper，但其实兼容性比较差。就算兼容比较好的Mapper如Mapper 4，居然连忍者神龟3都无法运行。
通过IDA Pro反编译其他部分的代码，发现虽然SNESticle支持FDS（需要将disksys.rom与fds放同一文件夹下），但对FDS的支持度非常差，绝大部分FDS无法运行，只有少量FDS可以运行。
