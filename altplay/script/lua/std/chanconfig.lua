--[[
  
  Per-channel configuration
  
]]

require"json"

local open = io.open

local function read_file(path)
    local file = open(path, "rb") -- r read mode and b binary mode
    if not file then return nil end
    local content = file:read "*a" -- *a or *all reads the whole file
    file:close()
    return content
end

local writetofile = function(f,c)
  file = open("script/lua/chanconfig/"..f, "w")
  file:write(c)
  file:close()
end

local module = {}
local channels = {}
module.set = function(info,what,to)
  what = what:split(".")
  local c = channels[info.msg.target]
  for k,v in pairs(what) do
    print(v)
    c = c[v]
    if type(c[v]) ~= "table" then break end
  end
  c = to
end

module.get = function(target,what)
  return channels[target][what]
end

local index = {}
local updateOnUpdateChannel = function(t,c)
  local proxy = {}
  proxy[index] = t
  return setmetatable(proxy, {
    __index = function(t,k) print("*access to element " .. tostring(k)); return updateOnUpdateChannel(t[index][k],c) end,
    __newindex = function(t,k,v) print("*update of element " .. tostring(k) .. " to " .. tostring(v)); rawset(t,k,v); writetofile(c..".json", json.encode(channels[c])) end
  })
end
local chanMt = function(t,c)
  return setmetatable(t, {
    __index = function(t,k)
      return updateOnUpdateChannel(rawget(t,"prototype")[k],c) or updateOnUpdateChannel({
        prefix = "",
        modules = updateOnUpdateChannel({
          youtube = updateOnUpdateChannel({
            enabled = true,
            search = true,
            random = true
          },c)
        },c)
      },c)[k]
    end
  })
end
module.new = function(info)
  print("got add")
  local newChan = {
    prefix = "",
    modules = {
      youtube = {
        enabled = true,
        search = true,
        random = true
      }
    }
  }
  channels[info.msg.target] = chanMt(newChan)
  writetofile(info.msg.target .. ".json", json.encode(newChan))
end

module.init = function()
  for _, v in ipairs(bot.listdir("script/lua/chanconfig")) do
    local chanfile, c = v:match("^((.+)%.[Jj][Ss][Oo][Nn])$")
    if chanfile then
      channels[c] = chanMt(json.decode(read_file("script/lua/chanconfig/" .. c .. ".json")))
    end
  end
end

return module