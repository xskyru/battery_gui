# 电池过欠压计算工具（Battery GUI）

这是一个用 C 语言 + GTK 实现的桌面图形界面小工具，用于电池参数计算。

## 💡 功能说明

- 输入额定电压、电池功率
- 自动计算电池数量
- 给出过压/欠压保护电压
- 显示母线电流估算值
- 占空比计算

## 📷 截图展示

> ![Image](https://github.com/user-attachments/assets/a6280b88-ff32-4e8d-b18c-3e1029ab750d)

> 后续可贴上截图说明界面界面

## 🚀 编译方式（Linux）

```bash
gcc `pkg-config --cflags gtk+-3.0` -o battery_gui battery_gui.c `pkg-config --libs gtk+-3.0`
```

## 运行方式

```c
./battery_gui

```

