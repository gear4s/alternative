--[[
  
  Table utitlies
  
  Mostly from lua-users.org
  
]]
do
  require"json"
  local decode = json.decode.decode
  local encode = json.encode.encode
  table.save = function(f,t)
    
  end

  table.print = function(tt, indent, done)
    done = done or {}
    indent = indent or 0
    if type(tt) == "table" then
      local sb = {}
      for key, value in pairs (tt) do
        sb[#sb] = string.rep(" ", indent) -- indent it
        if type(value) == "table" and not done[value] then
          done [value] = true
          sb[#sb] = "{\n"
          sb[#sb] = table.print(value, indent + 2, done)
          sb[#sb] = string.rep(" ", indent) -- indent it
          sb[#sb] = "}\n"
        elseif "number" == type(key) then
          sb[#sb] = string.format("\"%s\"\n", tostring(value))
        else
          sb[#sb] = string.format("%s = \"%s\"\n", tostring(key), tostring(value))
         end
      end
      return table.concat(sb)
    else
      return tt .. "\n"
    end
  end
end