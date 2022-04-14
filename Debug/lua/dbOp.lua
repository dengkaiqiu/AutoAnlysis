
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
	 
	if(nil == db) then
		return dbName.."open error"
	end
	
	return ""
end

function _M.search(sql)
	if(nil == db) then
		return nil
	end
	
	sqltoUtf8 = convert.to_utf8(sql);
	db:exec(sqltoUtf8,showrow,'test_udata')
end

function _M.prepare(sql)
	
	if(nil == db) then
		return nil
	end
	
	sqltoUtf8 = convert.to_utf8(sql);
	select_stmt = db:prepare(sqltoUtf8);
end

function _M.getRow()
	
	if(nil == select_stmt) then
		return nil
	end
		
	local stepret = select_stmt:step()
	if stepret == sqlite3.ROW then
		local ret = select_stmt:get_values() --返回的是一个lua array
		return ret
	else
		select_stmt:reset() --重置statement，以便下一次从表头开始查询
		return nil
	end
end	


function _M.close()
	if(nil == db) then
		return nil
	end
	db:close();
end
	
return _M
 