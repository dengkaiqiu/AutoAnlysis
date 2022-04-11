local dbOp = require("dbOp")
local convert = require("convertor")
local bigfile = require("bigfilemap")
local _g = require("globalData")
local eb_branch1 = require("eb_branch1")
--local eb_branch2 = require("eb_branch2")



--local dbName = "20211225_22.13.20.db";
--local table_name = "train";
--local sql = "SELECT "..[[���к�]]..","..[[������Ϣ�ܳ���]].." from "..table_name.." where "..[[���]].."=='1'";


print("==========================��������ʾDemo===========================")


--�����Զ�����֮��ʼ������
function tool_init(str)
  
	--��ʼ������
	ret,errmsg = _g.Init(str);

	--���Ӧ����־
	--ret,errmsg = _g.splitApplog();

	--���ƽ̨��־
	--ret,errmsg = _g.splitPlatLog();
	
	--��������
	return {ret=ret,errmsg=errmsg}
end

function auto_eb_branch1(str)
	
	--��ʼ������
	eb_branch1.init(str);
	
	--���Ӧ����־
	ret,errmsg = _g.splitApplog();

	--���ƽ̨��־
	ret,errmsg = _g.splitPlatLog();

	ret,errmsg = eb_branch1.anlysis();
	
	--��������
	return {ret=ret,errmsg=errmsg}
end


function auto_eb_branch2(str)
	print("auto_eb_branch2");
	
	--��������
	return {ret="0x55",errmsg=""}
end






















--��ʾ��ѯ���[name..value]
function test_dbSearch(str)
	dbOp.new(str.dbName);
	dbOp.search(sql);
	dbOp.close();
	return {ret="0x55"}
end

--��ʾ��ѯ���[col1...coln]
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

--��������lua�еľֲ�����
function test_setdbName(str)
	dbName = str.dbName;
	return {ret="0xaa"}
end

--��ȡ����lua�еľֲ�����
function test_getdbName()
	return {ret="0x55",dbName = dbName}
end


--���Դ��ļ����
function test_bigfileRead(str)
	local applog = str.applog;
	local platlog = str.platlog;
    bigfile.new({"applog"});
	local errmsg = bigfile.openfile({applog=applog,platlog=platlog})

	if(string.len(errmsg)) then
		print("==============��ʼ���Ӧ����־==============");
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

--���԰�����ȫ����ȡ����
function test_bigFileRead(errmsg)

	if(string.len(errmsg)) then
		print("==============���Ӧ����־�ɹ�==============");
		errmsg,cnt = bigfile.getFrameCnt("applog");
		print("���������֡����:"..cnt);
		minid,maxid = bigfile.getFrameLimit("applog");
		print("����֡��Χ:["..minid.."-"..maxid.."]");
		print("==============��ȡָ������9023446��־==============");
		errmsg,logmsg = bigfile.getCycleInfo("applog","9023446");
		print(logmsg);
		print("==============��ȡָ������9023450��־==============");
		errmsg,logmsg = bigfile.getCycleInfo("applog","9023450");
		print(logmsg);
	else
		print(errmsg);
	end

	return {ret="0x55"}
end


--���Դ��ļ����ж�ȡ
function test_bigfileReadRow(errmsg)

	if(string.len(errmsg)) then
		print("==============���Ӧ����־�ɹ�==============");
		errmsg,cnt = bigfile.getFrameCnt("applog");
		print("���������֡����:"..cnt);
		minid,maxid = bigfile.getFrameLimit("applog");
		print("����֡��Χ:["..minid.."-"..maxid.."]");
		print("==============��ȡָ������9023446��־==============");
		
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







