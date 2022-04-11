--ȫ�ֱ��������

_G ={}

_G.bigfile = require("bigfilemap");  --���ļ�����
_G.dbOp = require("dbOp");      --���ݿ����

--��ʼ������
function _G.Init(str)
	--��ʼ������
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

--���Ӧ����־
function _G.splitApplog()
	local errmsg="";
	
	if _G.appSplitStatus then 
		return 0x55,errmsg
	end
	
	--��Ҫ��ֵ���־����
	_G.bigfile.new({"applog"});
	
	--������־·��
	errmsg = errmsg.._G.bigfile.openfile({applog=_G.applog})

	--���Ӧ����־
	if(string.len(errmsg)) then
		print("==============��ʼ���Ӧ����־==============");
		print("keystr:".._G.appSplitKeystr);
		print("matchstr:".._G.appSplitMatchstr);
		errmsg = errmsg.._G.bigfile.splitfile("applog", _G.appSplitKeystr, _G.appSplitMatchstr)

    else
		errmsg = errmsg.."\n";
	end

	--��ʾӦ����־�����Ϣ
	if(string.len(errmsg)) then
		print("==============���Ӧ����־�ɹ�==============");
		errmsg,cnt = _G.bigfile.getFrameCnt("applog");
		print("���������֡����:"..cnt);
		minid,maxid = _G.bigfile.getFrameLimit("applog");
		print("����֡��Χ:["..minid.."-"..maxid.."]");
		_G.appSplitStatus = true;
	else
		errmsg = errmsg.."\n";
	end
	
	if(string.len(errmsg)) then 
		return 0x55,errmsg
	end
	
	return 0xaa,errmsg
end

--���ƽ̨��־
function _G.splitPlatLog()
	local errmsg="";
	
	if _G.platSplitStatus then 
		return 0x55,errmsg
	end
	
	--��Ҫ��ֵ���־����
	_G.bigfile.new({"platlog"});
	
	--������־·��
	errmsg = errmsg.._G.bigfile.openfile({platlog=_G.platlog})

	--���ƽ̨��־
	if(string.len(errmsg)) then
		print("==============��ʼ���ƽ̨��־==============");
		print("keystr:".._G.platSplitKeystr);
		print("matchstr:".._G.platSplitMatchstr);
		errmsg = errmsg.._G.bigfile.splitfile("platlog", _G.platSplitKeystr, _G.platSplitMatchstr)

    else
		errmsg = errmsg.."\n";
	end

	--��ʾƽ̨��־�����Ϣ
	if(string.len(errmsg)) then
		print("==============���ƽ̨��־�ɹ�==============");
		errmsg,cnt = _G.bigfile.getFrameCnt("platlog");
		print("���������֡����:"..cnt);
		minid,maxid = _G.bigfile.getFrameLimit("platlog");
		print("����֡��Χ:["..minid.."-"..maxid.."]");
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

--�л�Ӧ����־
function _G.changeApplog(str)
	local errmsg="";
	
	--�ر�֮ǰ�ľ��
	errmsg = _G.bigfile.close("applog");
	_G.appSplitStatus = false;
	
	if(nil ~= string.find(errmsg,"not exit key:",1)) then 
		--���²���
		_G.applog = str.applog;
		_G.appSplitKeystr = str.appSplitKeystr;
		_G.appSplitMatchstr = str.appSplitMatchstr;
		
		return _G.splitApplog();
	end
	
	return "0xaa",errmsg
end

--�л�Ӧ����־
function _G.changePlatlog(str)
	local errmsg="";
	
	--�ر�֮ǰ�ľ��
	errmsg = _G.bigfile.close("platlog");
	_G.platSplitStatus = false;
	
	if(nil ~= string.find(errmsg,"not exit key:",1)) then 
		--���²���
		_G.platlog = str.platlog;
		_G.platSplitKeystr = str.platSplitKeystr;
		_G.platSplitMatchstr = str.platSplitMatchstr;
		
		return _G.splitPlatLog();
	end
	
	return "0xaa",errmsg
	
end

--��������־�������ı�
function _G.exportTxt(filename,info)
	local f = io.open(filename,"a");
	f:write(info);
	f:flush();
	f:close();
end


return _G
