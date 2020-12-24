# PHP公历转农历扩展
## Quick start(快速入门)
1. Install(安装)
   ```
   git clone https://github.com/2233466866/lunar.git
   phpize
   ./configure
   make
   make install
   ```

1. Usage(使用)
   ```
   # CLI
   php -r "echo date_to_lunar(2000,1,1);"
   # CODE
   echo date_to_lunar(2000,1,1);
   ```

1. PS(说明)
  ```
  年份参数必须为1970-2100,否则会报error，或者结果计算错误。
  ```
