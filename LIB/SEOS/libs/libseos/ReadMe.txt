2017/4/29 Lydia
1.打开屏幕的非接灯
2.修正6210非接灯显示不正常的情况
3.增加6210纯USB口，为ttyGS1
4.增加cl_led_support(uint8_t IsSupport);//支持非接屏幕灯设置
版本号为1.1.9

2017/2/10 Lydia
1.修正拼音输入法输到第7个汉字乱码的问题
2.增加函数void SetCharType(uint8_t charType)，修正打印和显示阿拉伯语溢出的问题。如果是中文显示或打印，charType设置为0，如果是阿拉伯语或者其他会溢出的，charType设置为1。默认为0
版本号为1.1.8

2016/3/5 Lydia
1.增加USB1到USB9，ACM1到ACM9
版本号为1.1.6

2016/2/25 Lydia
1.增加prnClearSurface函数，清除打印surface，不重新上电模块，因为我们只能连续打印125行（海外需求）
版本号为1.1.5

2015/9/24 Lydia
1.打开磁头增加返回值
版本号为1.1.4

2015/8/28 Lydia
1.增加int prnCut(uint32_t cut_mode);切纸函数，只适用BG21图市农行代缴费项目。
版本号为1.1.3

2015/7/11 Lydia
1.修改打印和显示换行汉字半角和最后一个多出一个汉字半角的问题，同时也解决乱码溢出的问题。
版本号为1.1.2

2015/6/15 Lydia
1.增加N68
版本号为1.1.1

2015/5/30 Lydia
1.增加N68的软关机键。
版本号为1.1.0

2015/5/1 Lydia
1.获取终端序列号改成支持20位，给支付宝的机器是12位。
版本号为1.1.0

2015/3/31 Lydia
1.增加获取版本号函数seos_get_libversion。
版本号为1.1.0

2015/3/23 Lydia
1.获取终端序列号改成支持10位，给AMP公司的定制机是10位的。

2015/2/10 Lydia
1.增加kbWaitCodeKey函数，没有经过directfb过滤，取到的是原始值。

2014/12/30 Lydia
1.增加/dev/hid设备。
2.增加彩色的显示。彩色显示和打印有冲突，打印的时候需要再调回黑白。彩色显示调用在应用程序调用lcdSetFgColorVariety，lcdSetBgColorVariety。并且增加DISP_COLOR

2014/12/15 Lydia
1.修改kbGetString，增加KB_EN_DECIMAL，处理三位小数点的情况。

2014/11/18 Lydia
1.application_exit这个函数增加dfb_destroty_font_by_file，之前没有删除字体，如果不重启，下次再进就会出错。

2014/11/11 Lydia
1.FILE_INFO结构体里面的name由50改回17，兼容随行付的时候改成了50，但是外面用的都是17，如果用50，所有的程序都要重新编译。
2.修改portOpen函数，兼容ttyusb和ttyacm口

2014/11/10 Lydia
1.修正打印缺纸不继续打印的问题。

2014/11/6 Lydia
1.兼容随行付的字体。
2.增加ACM口。

2014/10/30 Lydia
1.修改prnStatus函数，1~7位都定义为错误的，第0位是总状态

2014/10/29 Lydia
1.增加printopen的判断，小于0退出，兼容6210是否有接底座。
2.7210打印增加是否外接电池的返回值。
3.sysReadConfig函数把sys_get_machine_config改成sys_get_hardware_config，方便以后扩展。

2014/9/5 Lydia
1.global_pntfd判断都改成>=0
2.seos库汉字输入函数，控制最大输入长度有个bug，当已输入长度等于max_len-1时还可以再输入一个汉字，但输入后长度超出了。
3.seos库汉字输入选择备用字时最左边的字符会被覆盖一部分，调整一点偏移。
4.增加NFC_LED_SUPPORT，如果为1说明支持屏显非接灯。
5.sysReadConfig函数增加7210的设备。

2014/7/28 Lydia
1.优化一些显示界面。
2.去掉非接灯。

2014/5/26 Lydia
1.增加非接灯。
2.增加6210的打印走纸。应用程序初始化步数，打印不满一页自动送纸。
3.修改kbGetString，字母键用功能键代替。
4.修改签名时校验值使用字体,应对针打分辨率小而打印不全的问题。
5.修正sysReadConfig，不区分大小写。
6.串口增加/dev/ttyGS0，/dev/ttyUSB0

20140220 Lydia
1.串口增加/dev/ttyS2

20140110 Lydia
1.刷卡的超时时间由10ms改为1ms,为了减少非接交易时间。
2.定时器的休眠时间改成10ms

2013/11/26 Lydia
1.修改kbGetString函数，之前的换行之后按清除键会有问题。
2.如果打印已经分配了内存，则prnInit不会重新分配。

2013/11/8 Lydia
1.prnStart函数里面不执行prnClose，应用程序调用prnStart之后还需要调用prnClose释放内存。

2013/10/28 Lydia
1.解决从控制台进有可能连不上的问题。
2.display_create(param.ro_width, 3000, 0, global_font_24, 24);3000*4*384=4.39M,占了4M多的内存，以前的做法是一启动程序就分配这么多内存，等到退出程序的时候才释放，这样导致程序内存占用率很高。现在改成打印的时候才分配内存，打印完成之后就释放，这样程序节省了4.39M的空间，同时运行两个程序是没问题的

2013/10/23 Lydia
1.修正kbGetString函数输入英文字体清屏不能清干净的问题。
2.KEY_INVALID和KEY_TIMEOUT之前的值均为0，为了区分，也为了和8110保持统一,KEY_INVALID值改为0xff,KEY_TIMEOUT的值还是为0

2013/10/8 Lydia
1.3G只能镜像1.0.6版本以及以上才能使用，外面有很多使用1.0.5的版本，如果是1.0.5的版本，修改seos之后gprs也无法使用，所以去掉了3G.

2013/9/27 Lydia
1.增加3G的实现，通过wnet_getmodinfo(&wnet_info);判断机器通讯类型，如果是3G，应用程序的通讯和GPRS是一样的。

2013/09/02
1.WnetInit 去掉wnet_power_off的代码，且wnet_power_on后由4s改成1s


2013/07/11 21：30
1.lcdDisplay的显示根据屏幕大小调整显示
2.修改kbGetHz的函数根据屏幕大小调整参数以便在8110P使用

2013/07/11
1.修改了用宏控制lcdDisplay，及修改lcdClrline

2013/06/24
1.PPPLogin和PPPLogout删除打开modem电源modem_power_control，因为sdk增加了这个接口
2.修改lcdSetFontExt和lcdGetFontExt的接口， 设置字体的同时设置字体高度

2013/06/22
1.修改lcdSetFont的以兼容以前程序
2.修改lcdDisplay会清上一行的线
3.modemPPP拨号不能打开modem的操作，修改了PPPLogin和PPPLogout

20013/06/21
1.修复lcdDisplay出现叠加现象的bug
2.增加lcdSetFontExt和lcdGetFontExt的接口
3.修改lcdSetFont指定设置字体的类型


2013/06/20
1.lcdDisplay去掉subsDurface并且用SetClip设置有效区域;
2.lcdDisplay增加DISP_FLIP的属性，控制局部刷新
3.windowResize函数增加SetClip设置有效区域的操作

2013/06/19
1.prnStart函数增加休眠
2.prnPrintf根据放大倍数决定是否要用放大函数
3.system_display_initialize中window获取创建事件缓冲区失败，将global_events赋空

2013/06/18
1.WnetReset的实现调用wnet_reset
2.增加局部刷新函数lcdFlipLine；

2013/06/09
1.修复fileRemoveDir的bug
2.修复拼音输入__kbGetHZ刚开始“拼音：”显示慢的问题

2013/06/07
1.修改了kbGetString的按键音问题
2.blit问题
3.获取wirless配置问题
4.magSwiped的系统中断问题
5.添加了fileMkDir， fileRemoveDir的函数
6.PPP函数中cdma的设备名

2013/05/27
1.添加创建目录和删除目录的函数

2013/05/20
1.pntDrawPicture的bug,此问题应该也存在7110中

2013/04/09
1.subsurface->flip() 修改成subsurface->Blit();

