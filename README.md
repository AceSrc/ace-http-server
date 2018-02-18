# 使用说明

配置一下 config 文件之后 make run 就好了, 注意一下如果要使用php-fpm的话记得确保php-fpm有权限访问到网站的根目录. 

目前 config 支持的配置命令有:

- max_user: 最大监听线程数
- port: 服务器开放端口
- index: 缺省时展现的网页
- error_page: 404 网页
- log_file: 网站日志存放文件
- root: 网站所在的根目录
- php, toy...: 对应后缀名的 Fastcgi 程序对应的端口

# 功能说明
## 并发队列实现的 Message Queue 提供健壮的日志系统
  
  主要利用 Message Queue 将缓慢的 IO 操作和服务器繁重的 HTTP 回应操作分离开来, 即服务器线程将日志信息丢进并发队列中即可进行接下来的操作, 由
  Writer类 不断地从队列中取出日志信息进而写入硬盘. 

  同时由于和服务器线程分离开来, 当服务器崩溃时仍能将队列中的日志逐一写入硬盘, 防止
  日志随着服务器的崩溃而消失, 方便故障查询. 

## Fastcgi 协议

  简单实现了 Fastcgi 协议, 目前可以和 php-fpm 通信, 从而实现动态解析 php 页面的功能. 

## .toy 页面

  .toy 是我写的一个动态渲染语言 (Extremely Tiny, a toy language), 仅支持整数类型和字符串, 可以进行简单的数学运算. 

  整个语言解释器是由 flex/bison 生成的. 

  对应的 Fastcgi 程序也写好放在了 toy 文件夹下, 默认监听 9001 端口. 

## cookie

  都可以用 php 了, 所以简单的 session control 就可以通过 php 的 setcookie 和
  _COOKIE 来实现了. 

## 已经有的页面
  letter 是我以前弄的一个 js 练习
  
  html 里面有 trust 和 blog 是作业要求显示的网页, 如果要显示记得 修改 config 里面的 root