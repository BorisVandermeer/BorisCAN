# CANalystII 使用说明

***方法一***

```bash
# 创建一个新的 udev 规则，名称取为：99-myusb.rules
sudo vi /etc/udev/rules.d/99-myusb.rules

# 添加
ACTION=="add",SUBSYSTEMS=="usb", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="0053",
GROUP="users", MODE="0777"


```

***方法二***

```bash
cd /etc/udev/rules.d
sudo touch 99-myusb.rules
sudo gedit 99-myusb.rules
## 将下面这两句代码复制到文件中
ACTION=="add",SUBSYSTEMS=="usb", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="0053",
GROUP="users", MODE="0777"
## 重启电脑即可生效
```
