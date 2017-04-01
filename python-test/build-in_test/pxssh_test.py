# -*-encoding=utf-8-*-
import pexpect
import pexpect.pxssh as pxssh
import getpass

# 第一步与终端建立连接
child = pexpect.spawn('sudo -i')
# # 第二步等待终端返回特定内容
a= child.expect('good')
# # 第三步根据返回内容发送命令进行交互
# child.sendline('123')