

本次仓库使用标准库-keil5 和 Hal库 CLion + CubeMX 开发

GPIO_hal里面是HAL库写的代码
GPIO_std里面是标准库写的代码
里面都有一个叫做void的空项目，项目仅仅配置了晶振和SWD下载端口，做其他工程时可以自己复制使用。
但是如果是复制并且改名了的话CLion需要更改一下编译配置，不然会找不到.cfg文件。
