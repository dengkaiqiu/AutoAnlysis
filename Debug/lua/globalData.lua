--全局变量存放区

_G ={}

_G.bigfile = require("bigfilemap");  --大文件操作
_G.dbOp = require("dbOp");      --数据库操作

--初始化操作
function _G.Init(str)
	--初始化参数
	_G.dbName = str.dbName;
	_G.applog = str.applog;
	_G.platlog = str.platlog;
	_G.appSplitKeystr = str.appSplitKeystr;
	_G.appSplitMatchstr = str.appSplitMatchstr;
	_G.appSplitStatus = false;
	_G.platSplitKeystr = str.platSplitKeystr;
	_G.platSplitMatchstr = str.platSplitMatchstr;
	_G.platSplitStatus = false;
	
	return "0x55",""
end

--拆分应用日志
function _G.splitApplog()
	local errmsg="";
	
	if _G.appSplitStatus then 
		return 0x55,errmsg
	end
	
	--需要拆分的日志类型
	_G.bigfile.new({"applog"});
	
	--设置日志路径
	errmsg = errmsg.._G.bigfile.openfile({applog=_G.applog})

	--拆分应用日志
	if(string.len(errmsg)) then
		print("==============开始拆分应用日志==============");
		print("keystr:".._G.appSplitKeystr);
		print("matchstr:".._G.appSplitMatchstr);
		errmsg = errmsg.._G.bigfile.splitfile("applog", _G.appSplitKeystr, _G.appSplitMatchstr)

    else
		errmsg = errmsg.."\n";
	end

	--显示应用日志拆分信息
	if(string.len(errmsg)) then
		print("==============拆分应用日志成功==============");
		errmsg,cnt = _G.bigfile.getFrameCnt("applog");
		print("共拆分数据帧数量:"..cnt);
		minid,maxid = _G.bigfile.getFrameLimit("applog");
		print("数据帧范围:["..minid.."-"..maxid.."]");
		_G.appSplitStatus = true;
	else
		errmsg = errmsg.."\n";
	end
	
	if(string.len(errmsg)) then 
		return 0x55,errmsg
	end
	
	return 0xaa,errmsg
end

--拆分平台日志
function _G.splitPlatLog()
	local errmsg="";
	
	if _G.platSplitStatus then 
		return 0x55,errmsg
	end
	
	--需要拆分的日志类型
	_G.bigfile.new({"platlog"});
	
	--设置日志路径
	errmsg = errmsg.._G.bigfile.openfile({platlog=_G.platlog})

	--拆分平台日志
	if(string.len(errmsg)) then
		print("==============开始拆分平台日志==============");
		print("keystr:".._G.platSplitKeystr);
		print("matchstr:".._G.platSplitMatchstr);
		errmsg = errmsg.._G.bigfile.splitfile("platlog", _G.platSplitKeystr, _G.platSplitMatchstr)

    else
		errmsg = errmsg.."\n";
	end

	--显示平台日志拆分信息
	if(string.len(errmsg)) then
		print("==============拆分平台日志成功==============");
		errmsg,cnt = _G.bigfile.getFrameCnt("platlog");
		print("共拆分数据帧数量:"..cnt);
		minid,maxid = _G.bigfile.getFrameLimit("platlog");
		print("数据帧范围:["..minid.."-"..maxid.."]");
		_G.platSplitStatus = true;
	else
		errmsg = errmsg.."\n";
		print(errmsg);
	end
	
	if(string.len(errmsg)) then 
		return 0x55,errmsg
	end
	
	return 0xaa,errmsg
end

--切换应用日志
function _G.changeApplog(str)
	local errmsg="";
	
	--关闭之前的句柄
	errmsg = _G.bigfile.close("applog");
	_G.appSplitStatus = false;
	
	if(nil ~= string.find(errmsg,"not exit key:",1)) then 
		--更新参数
		_G.applog = str.applog;
		_G.appSplitKeystr = str.appSplitKeystr;
		_G.appSplitMatchstr = str.appSplitMatchstr;
		
		return _G.splitApplog();
	end
	
	return "0xaa",errmsg
end

--切换应用日志
function _G.changePlatlog(str)
	local errmsg="";
	
	--关闭之前的句柄
	errmsg = _G.bigfile.close("platlog");
	_G.platSplitStatus = false;
	
	if(nil ~= string.find(errmsg,"not exit key:",1)) then 
		--更新参数
		_G.platlog = str.platlog;
		_G.platSplitKeystr = str.platSplitKeystr;
		_G.platSplitMatchstr = str.platSplitMatchstr;
		
		return _G.splitPlatLog();
	end
	
	return "0xaa",errmsg
	
end

--将周期日志导出到文本
function _G.exportTxt(filename,info)
	local f = io.open(filename,"a");
	f:write(info);
	f:flush();
	f:close();
end


return _G
