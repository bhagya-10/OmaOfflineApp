修改记录：
2017/10/30 liluchang
1.更新libsbTms.so库，解决tms任务更新时间过长，而ui界面报错的问题；
2.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M2.4/170708         2.4.1
sbTmsMain       V2.4/170708         2.4.1 
libnewTms.so    V2.4.0/170321       2.4.0
libsbTms.so     V1.2.1/170321       2.4.1
3.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2017/07/10 liluchang
1.适应银联21号的处理要求，TMS SN字段最大可达50字节，之前预留的空间不够，这个版本增加兼容；
2.修复问题，在执行sys_get_ans之前先判断镜像版本;
3.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M2.4/170708         2.4.1
sbTmsMain       V2.4/170708         2.4.1 
libnewTms.so    V2.4.0/170321       2.4.0
libsbTms.so     V1.2.1/170321       2.4.0
4.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2017/04/13 liluchang
1.发布支持SSL模式的TMS主控和TMS下载库；灌装程序的时候，通过HostIp.ini文件中的SSL_ENABLE字段进行配置；
2.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M2.4/170321         2.4.0
sbTmsMain       V2.4/170321         2.4.0 
libnewTms.so    V2.4.0/170321       2.4.0
libsbTms.so     V1.2.1/170321       2.4.0
3.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2017/01/13 liluchang
1.修复了子应用手动发起tms下载，下载百分比一直为0.0%的问题，实际是在下载中，只修改了libnewTms.so；其他不变
2.因为要涉及到升级，而libnewTms.so与Manage应用绑定在一个pkg，故Manage的版本号要上升；
3.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M2.3/161208          2.3.2
sbTmsMain       V2.3/161206          2.3.1 
libnewTms.so    V1.2/161111          2.3.1
libsbTms.so     V1.2.0/161111        2.3.0
4.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2017/01/09 liluchang
1.更新Manage、sbTmsMain、libnewTms.so、libsbTms.so文件；
2.优化了主控输入ip的方式；
3.优化了CDMA拨号参数；
4.优化了wifi通讯时，做tcp连接前先检查是否成功连上wifi热点；
5.兼容单中的多tms系统的处理；（多个子应用、无主控、各自带sbTmsMain运行，每个sbTmsMain对接一个tms系统）
6.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M2.3/161208          2.3.1
sbTmsMain       V2.3/161206          2.3.1 
libnewTms.so    V1.2/161111          2.3.0
libsbTms.so     V1.2.0/161111        2.3.0
7.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2016-11-10 liluchang
1.提交发布最新版本银盛tms的相关程序，最新的Manage、sbTmsMain、libsbTms.so、libnewTms.so
2.银盛tms与之前的版本，最大的区别就是，它不用配置任务，巡检时根据版本高低自动匹配任务下载，无需人工手动配置任务；
3.当使用裸主控的时候，可支持配置N个子应用下载（配置下载任务）；
4.新的web管理平台，支持更多终端的信息的展示；
5.注意：普通版本的tms，请使用上一目录的程序
6.版本信息
程序名           应用版本号       签名版本号
Manage          M1.3/160926         1.4.4
sbTmsMain       V1.2/160926         1.2.4
libsbTms.so     V1.1/160830         1.1.3          
libnewTms.so    V1.1.3/160830       1.1.3  

2016/08/08 liluchang
1.更新了libsbTms.so和libnewTms.so；主要是新增若干TMS服务器添加的返回码解析
2.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160617          1.3.2
sbTmsMain       V1.1/160511          1.1.1 
libnewTms.so    V1.1/160808          1.1.2
libsbTms.so     V1.1/160808          1.1.2
3.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2016/07/25 liluchang
1.调整后台进程启动的顺序，现在让它在扫描完子应用列表后启动；
2.如果是由于等待输入按键输入超时而从“TMS管理器”界面切换到下一界面，则不提示“主控退出”；否则从“TMS管理器”界面按取消键，会提示主控退出选择界面；
3.调整ys TMS的识别文件由 "ys_manage_ver_cfg" 变成 "ys_manage_ver.cfg.special"
4.多应用设置按键等待时间过长，已设置为stPosParam.ucOprtLimitTime，默认是60s
5.版本号 M1.3/160713
6.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160713          1.0.3

2016/07/08 liluchang
1.提交一个银盛屏蔽tms的版本，配合银盛四合一程序使用的主控；主要是初始化的时候不要 一直打开按键的使能开关
有时候是底层管理器中关闭了这个按键声音使能开关。
2.删除了部分 无效代码
3.版本号 M1.3/160708
4.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160708          0.0.0

2016/07/04 liluchang
1.提交新版本主控程序；
2.主要修改点为，主控手动发起远程下载，可动态刷新进度条；
1）.修复部分显示在8110p黑白屏上显示不全的bug；
2）.系统维护密码由原来的固定8位长度，改为4-8位；
3）.HostIp.ini增加默认系统维护密码的设置项；可灵活配置系统维护的初始密码；
4）.HostIp.ini增加多应用管理器界面等待用户选择启动应用的最长时间；
5）.增加检查子应用返回的tms参数路径是否为空；
6）.退出主控应用的提示菜单，放在TMS管理器菜单之后，即 应用管理器菜单->TMS管理器菜单->退出应用菜单；
7）.增加与子应用交互的TAG 0xF2，标识主控当前是否关闭tms功能，默认tms是打开的；
3.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160617          1.3.2
sbTmsMain       V1.1/160511          1.1.1 
libnewTms.so    V1.1/160511          1.1.1
libsbTms.so     V1.1/160511          1.1.1
4.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。

2016/06/06 liluchang
1.提交最新版本的tms动态库及主控程序、后台进程；
2.主要修改点为，主控手动发起远程下载，可动态刷新进度条；
3.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160511          1.3.1
sbTmsMain       V1.1/160511          1.1.1 
libnewTms.so    V1.1/160511          1.1.1
libsbTms.so     V1.1/160511          1.1.1
4.备注，请务必保证使用的以上4个文件都是最新版本，配套使用。


2016/06/03 liluchang
1.提交最新版本tms动态库；签名版本为0.0.0
2.主要更新为，支持子应用在手动发起远程下载时，可调用接口查看当前下载的百分比；

2016/05/04 liluchang
1.提交最新版本的主控、后台进程、tms动态库等；
2.主控默认的TMS服务器参数通过HostIp.ini文件配置；
3.主控和后台进程支持域名访问；
4.银盛多应用需要，要求主控支持修改默认应用、启动默认应用等待时间，隐藏部分应用等需求；
5.增加主控显示背景图片；
6.特别提醒，为避免由于之前版本的bug引起后续开发功能，请务必使用最新的所有动态库和应用程序
（libnewTms.so、libsbTms.so、Manage和sbTmsMain）！！！！
7.程序均为普通签名版本。
程序名           应用版本号       签名版本号
Manage          M1.3/160429          1.3.0
sbTmsMain       V1.1/160429          1.1.0 
libnewTms.so    V1.1/160426          1.1.0
libsbTms.so     V1.1/160426          1.1.0

2016/03/07 liluchang
1.提交最新版本的tms主控程序，做了部分优化，尤其是后台进程的实现部分。
2.特别提醒，为避免由于之前版本的bug引起后续开发功能，请务必使用最新的所有动态库和应用程序（libtms.so、
libsbTms.so、Manage和sbTmsMain）！！！！
3.程序均为普通签名版本。

2016/01/23 liluchang
1.特别提醒，为避免由于之前版本的bug引起后续开发功能，请务必使用最新的所有动态库和应用程序（libtms.so、
libsbTms.so、Manage和sbTmsMain）！！！！

2016/01/07 liluchang
1.该目录下的4个可执行程序（2个app， 2个动态库），为整改后的新TMS主控程序，对比之前的版本程序，引入了后台进程；
2.在使用该主控程序时，务必保证Manage和sbTmsMain位于同一个Manage目录，同时务必使用新版本的libtms.so,及新增的
  与sbTmsMain通讯的libsbTms.so动态库；否则可能会导致进程间通讯失败；
3.pkg包格式可参考，本目录下的图片示例；
4.该程序已在支付宝项目中使用；有需求的可以咨询支付宝相关项目；
5.版本号：
1）.主控程序版本号M1.2/151015
2）.后台进程版本号V1.0/151015
3）.默认ip信息为，云ip/端口 121.40.187.133 : 38899
