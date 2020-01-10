为防止动态库命名冲突，
libtms.so已更名为libnewtms.so；
若应用中还依赖旧的libtms.so，则使用libtms.so动态库；否则一律要求使用libnewtms.so