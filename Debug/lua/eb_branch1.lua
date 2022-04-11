
local _g = require("globalData")  --ȫ�ֱ�����
local convert = require("convertor")  --�ַ���
local dbExport = require("dbExport")

local _eb_branch1 = {}


_eb_branch1.dbOp = require("dbOp")  
_eb_branch1.tablename = "train"

--��ʼ��
function _eb_branch1.init(str)
		_eb_branch1.devID = str.devID;
		_eb_branch1.devIDDec = convert.as_Uint(_eb_branch1.devID,16);
		print(_eb_branch1.devID)
end

--��ȡ��������
function _eb_branch1.hit()
	local index =1;
	local hitcycle = {};
	local hitflag = 0;
	local lastcycle = 0;
	local curcycle =0;
	
	--�����ݿ�
	_g.dbOp.new(_g.dbName);
		
	local sql = "SELECT "..[[���к�]]..","..[[�г�����]].." from ".._eb_branch1.tablename.." where "..[[�г�ID]].."=='".._eb_branch1.devID.."'";
	
	_g.dbOp.prepare(sql);
	
	local row = _g.dbOp.getRow()
	while(row ~= nil)
	do
		--��ס��ǰ����
		curcycle = convert.as_Int(row[1],10);
		
		--��ʼ����һ����
		if(lastcycle == 0) then
			lastcycle = curcycle - 1;
		end
		
		
		--�ж��Ƿ����
		if (lastcycle + 1 == curcycle) then
			if (0 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					hitflag = 1;  --����������
				end
			elseif (1 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					;  --�����ȶ�״̬
				else
					hitflag = 0;  --������״̬
				end
			else
				;  --δ������з�Χ
			end
	    else
			if(1 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					;  --�����ȶ�״̬
				else
					table.insert(hitcycle,""..(lastcycle+1));  --������ɾ��
				end
			end
		end
		
		lastcycle = curcycle;  --����������
		
		row = _g.dbOp.getRow();  --��ȡ��һ��
	end
	
	--�ر����ݿ�
	_g.dbOp.close();
	
	return hitcycle
end

--������־
function _eb_branch1.anlysis()
	local ret ="0xaa";
	local errmsg = "";
	local hitcycle = _eb_branch1.hit();
	
	--��ʾ����
	for k,v in ipairs(hitcycle) do 
		line,msg = _g.bigfile.getCycleRowInfo("applog",v);
		while(nil ~= line)
		do
			if(nil ~= string.find(line,"101.111",1)) then
				trainid,reservetrainid,acid = string.match(line,"101.111 ErrorCode=0x08000000 AC has a resverse train! The train is (.*),The next train is (.*),The AC is (.*)");
				
				if(convert.as_Uint(trainid,10) == _eb_branch1.devIDDec) then
					ret = "0x55";
					errmsg = errmsg..line.."\n";
					errmsg = errmsg.."�г�"..trainid.."���ڼ�������ǰ�����ڶ����г�"..reservetrainid.."����������Ϊ"..acid.."\n";
				end
			end
			line,msg = _g.bigfile.getCycleRowInfo("applog",v);
		end
		
		if("0x55" == ret) then
			print(errmsg);
			--errmsg,logmsg = _g.bigfile.getCycleInfo("applog",v);
			
			--_g.exportTxt(v.."applog.txt",logmsg);   --��������Ӧ����־�ı�
			--errmsg,logmsg = _g.bigfile.getCycleInfo("platlog",v);
			--_g.exportTxt(v.."platlog.txt",logmsg);   --��������ƽ̨��־�ı�
			
			--print(logmsg);
			--errmsg = dbExport.writeJson(_g.dbName,v);  --���������ڴ�
			--print(errmsg);
		end
		
	end
	
	--�ٴ�
	
	
	
	return ret,errmsg
end

return _eb_branch1
