
local sqlite3 = require("lsqlite3complete")
local convert = require("convertor")

local _M = {}
local db;
local select_stmt;

function showrow(udata,cols,values,names)
  assert(udata=='test_udata')
  --print('exec:')
  for i=1,cols do 
	--print('',convert.to_string(names[i]),values[i])
  end
  return 0
end

function _M.new(dbName)
	db = sqlite3.open(dbName)
end

function _M.search(sql)
	sqltoUtf8 = convert.to_utf8(sql);
	db:exec(sqltoUtf8,showrow,'test_udata')
end

function _M.prepare(sql)
	sqltoUtf8 = convert.to_utf8(sql);
	select_stmt = db:prepare(sqltoUtf8);
end

function _M.getRow()
	local stepret = select_stmt:step()
	if stepret == sqlite3.ROW then
		local ret = select_stmt:get_values() --���ص���һ��lua array
		return ret
	else
		select_stmt:reset() --����statement���Ա���һ�δӱ�ͷ��ʼ��ѯ
		return nil
	end
end	


function _M.close()
	db:close();
end
	
return _M
 