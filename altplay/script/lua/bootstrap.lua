--ease require"my.module"
package.path = "./script/lua/?.lua;" .. package.path

--lua 5.3 workaround
loadstring = loadstring or load

local startTime = os.time()

local urandom, entropy = io.open("/dev/urandom"), 0
if not urandom then entropy = os.time()
else
  for _ = 1,7 do entropy = entropy * 0x10 + urandom:read(1):byte() end
  urandom:close()
end
math.randomseed(entropy % 0x7FFFFFFF)
--
--simple module loader
local loadd = {}
for _, v in ipairs(bot.listdir("script/lua/modules.d")) do
  local load = {v:match("^((%d+)%-.+%.[Ll][Uu][Aa])$")}
  if #load == 2 then
    load[2] = tonumber(load[2])
    table.insert(loadd, load)
  end
end

table.sort(loadd, function(a, b) if a[2] ~= b[2] then return a[2] < b[2] else return a[1] < b[1] end end)
for _, v in ipairs(loadd) do dofile("script/lua/modules.d/"..v[1]) end
