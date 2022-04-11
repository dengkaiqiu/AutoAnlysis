
local _g = require("globalData")  --全局变量池
local convert = require("convertor")  --字符串
local dbExport = require("dbExport")

local _eb_branch1 = {}


_eb_branch1.dbOp = require("dbOp")  
_eb_branch1.tablename = "train"

--初始化
function _eb_branch1.init(str)
		_eb_branch1.devID = str.devID;
		_eb_branch1.devIDDec = convert.as_Uint(_eb_branch1.devID,16);
		print(_eb_branch1.devID)
end

--获取击中周期
function _eb_branch1.hit()
	local index =1;
	local hitcycle = {};
	local hitflag = 0;
	local lastcycle = 0;
	local curcycle =0;
	
	--打开数据库
	_g.dbOp.new(_g.dbName);
		
	local sql = "SELECT "..[[序列号]]..","..[[列车类型]].." from ".._eb_branch1.tablename.." where "..[[列车ID]].."=='".._eb_branch1.devID.."'";
	
	_g.dbOp.prepare(sql);
	
	local row = _g.dbOp.getRow()
	while(row ~= nil)
	do
		--记住当前周期
		curcycle = convert.as_Int(row[1],10);
		
		--初始化上一周期
		if(lastcycle == 0) then
			lastcycle = curcycle - 1;
		end
		
		
		--判断是否击中
		if (lastcycle + 1 == curcycle) then
			if (0 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					hitflag = 1;  --进入上升沿
				end
			elseif (1 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					;  --进入稳定状态
				else
					hitflag = 0;  --不满足状态
				end
			else
				;  --未进入击中范围
			end
	    else
			if(1 == hitflag) then
				if nil ~= string.find(row[2],"0x01",1) then
					;  --进入稳定状态
				else
					table.insert(hitcycle,""..(lastcycle+1));  --进行了删车
				end
			end
		end
		
		lastcycle = curcycle;  --更新上周期
		
		row = _g.dbOp.getRow();  --读取下一行
	end
	
	--关闭数据库
	_g.dbOp.close();
	
	return hitcycle
end

--分析日志
function _eb_branch1.anlysis()
	local ret ="0xaa";
	local errmsg = "";
	local hitcycle = _eb_branch1.hit();
	
	--显示错误
	for k,v in ipairs(hitcycle) do 
		line,msg = _g.bigfile.getCycleRowInfo("applog",v);
		while(nil ~= line)
		do
			if(nil ~= string.find(line,"101.111",1)) then
				trainid,reservetrainid,acid = string.match(line,"101.111 ErrorCode=0x08000000 AC has a resverse train! The train is (.*),The next train is (.*),The AC is (.*)");
				
				if(convert.as_Uint(trainid,10) == _eb_branch1.devIDDec) then
					ret = "0x55";
					errmsg = errmsg..line.."\n";
					errmsg = errmsg.."列车"..trainid.."所在计轴区段前方存在对向列车"..reservetrainid.."，计轴区段为"..acid.."\n";
				end
			end
			line,msg = _g.bigfile.getCycleRowInfo("applog",v);
		end
		
		if("0x55" == ret) then
			print(errmsg);
			--errmsg,logmsg = _g.bigfile.getCycleInfo("applog",v);
			
			--_g.exportTxt(v.."applog.txt",logmsg);   --导出周期应用日志文本
			--errmsg,logmsg = _g.bigfile.getCycleInfo("platlog",v);
			--_g.exportTxt(v.."platlog.txt",logmsg);   --导出周期平台日志文本
			
			--print(logmsg);
			--errmsg = dbExport.writeJson(_g.dbName,v);  --导出周期内存
			--print(errmsg);
		end
		
	end
	
	--再次
	
	
	
	return ret,errmsg
end

return _eb_branch1
