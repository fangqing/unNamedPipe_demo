该demo从 https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output 
演变而来,仅仅演示如何通过匿名管道在父子进程中进行通讯.

编译运行unNamedPipe.exe, 它会创建子进程"unamePipeChild.exe",
在 unNamedPipe 进程的console中 输入任意内容, unamePipeChild 子进程会告诉 unNamedPipe 进程
刚刚你输入了多少个字符,字符到内容是什么.
"you just input %d character:%s\n"
直到你输入"quit"后游戏才结束
