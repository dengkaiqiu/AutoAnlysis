local dbOp = require("dbOp")
local convert = require("convertor")
local bigfile = require("bigfilemap")
local _g = require("globalData")
local eb_branch1 = require("eb_branch1")
--local eb_branch2 = require("eb_branch2")



--local dbName = "20211225_22.13.20.db";
--local table_name = "train";
--local sql = "SELECT "..[[序列号]]..","..[[计轴信息总长度]].." from "..table_name.." where "..[[序号]].."=='1'";


print("==========================故障树演示Demo===========================")


--测试自动分析之初始化参数
function tool_init(str)
  
	--初始化参数
	ret,errmsg = _g.Init(str);

	--拆分应用日志
	--ret,errmsg = _g.splitApplog();

	--拆分平台日志
	--ret,errmsg = _g.splitPlatLog();
	
	--函数返回
	return {ret=ret,errmsg=errmsg}
end

function auto_eb_branch1(str)
	
	--初始化参数
	eb_branch1.init(str);
	
	--拆分应用日志
	ret,errmsg = _g.splitApplog();

	--拆分平台日志
	ret,errmsg = _g.splitPlatLog();

	ret,errmsg = eb_branch1.anlysis();
	
	--函数返回
	return {ret=ret,errmsg=errmsg}
end


function auto_eb_branch2(str)
	print("auto_eb_branch2");
	
	--函数返回
	return {ret="0x55",errmsg=""}
end






















--显示查询结果[name..value]
function test_dbSearch(str)
	dbOp.new(str.dbName);
	dbOp.search(sql);
	dbOp.close();
	return {ret="0x55"}
end

--显示查询结果[col1...coln]
function test_dbGetRow(str)
	dbOp.new(str.dbName);
	dbOp.prepare(sql);
	local row = dbOp.getRow()
	while(row ~= nil)
	do
		for k,v in ipairs(dbOp.getRow()) do 
			--print(v);
		end
		row = dbOp.getRow();
	end
	dbOp.close();

	return {ret="0x55"}
end

--测试设置lua中的局部变量
function test_setdbName(str)
	dbName = str.dbName;
	return {ret="0xaa"}
end

--获取设置lua中的局部变量
function test_getdbName()
	return {ret="0x55",dbName = dbName}
end


--测试大文件拆分
function test_bigfileRead(str)
	local applog = str.applog;
	local platlog = str.platlog;
    bigfile.new({"applog"});
	local errmsg = bigfile.openfile({applog=applog,platlog=platlog})

	if(string.len(errmsg)) then
		print("==============开始拆分应用日志==============");
		print("keystr:".."=====period");
		print("matchstr:".."=====period (.*) ZC App Start===== at (.*)");
		errmsg = bigfile.splitfile("applog", "=====period", "=====period (.*) ZC App Start===== at (.*)")
    else
		print(errmsg);
	end
		
	--test_bigFileRead(errmsg);
	test_bigfileReadRow(errmsg);

	return {ret="0x55"}
end

--测试按周期全部读取数据
function test_bigFileRead(errmsg)

	if(string.len(errmsg)) then
		print("==============拆分应用日志成功==============");
		errmsg,cnt = bigfile.getFrameCnt("applog");
		print("共拆分数据帧数量:"..cnt);
		minid,maxid = bigfile.getFrameLimit("applog");
		print("数据帧范围:["..minid.."-"..maxid.."]");
		print("==============获取指定周期9023446日志==============");
		errmsg,logmsg = bigfile.getCycleInfo("applog","9023446");
		print(logmsg);
		print("==============获取指定周期9023450日志==============");
		errmsg,logmsg = bigfile.getCycleInfo("applog","9023450");
		print(logmsg);
	else
		print(errmsg);
	end

	return {ret="0x55"}
end


--测试大文件按行读取
function test_bigfileReadRow(errmsg)

	if(string.len(errmsg)) then
		print("==============拆分应用日志成功==============");
		errmsg,cnt = bigfile.getFrameCnt("applog");
		print("共拆分数据帧数量:"..cnt);
		minid,maxid = bigfile.getFrameLimit("applog");
		print("数据帧范围:["..minid.."-"..maxid.."]");
		print("==============获取指定周期9023446日志==============");
		
		line,msg = bigfile.getCycleRowInfo("applog","9023446");
		while(nil ~= line)
		do
			print(line)
			line,msg = bigfile.getCycleRowInfo("applog","9023446");
		end
	else
		print(errmsg);
	end

	return {ret="0x55"}
end







