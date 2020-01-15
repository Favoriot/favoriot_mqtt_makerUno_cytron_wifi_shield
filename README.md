# Favoriot Mqtt maker uno cytron wifi shield
tutorial to send data stream using mqtt protocol

# Requirements: 
  arduino maker uno + wifi shield esp8266
  
  link: https://my.cytron.io/p-nodemcu-lua-v3-esp8266-wifi-with-ch340c?r=1&gclid=EAIaIQobChMIoMLJ4deE5wIVzA0rCh0NeA5FEAQYASABEgLpivD_BwE
  
# Topic:
mqtt publish : yourapikey + '/v2/streams'

mqtt subscribe : yourapikey + '/v2/streams/status'
  
# In this code, you need to change TWO things:
 *    1. Your 'Access Token' (can be obtained from devices in favoriot platform)
 *    2. Your device developer ID (can be obtained from devices in favoriot platform)
 
 # how to get access token
 **link: https://platform.favoriot.com/tutorial/v2/#device-access-token**
 
 # Issues or feedback
 **link: https://www.favoriot.com/forum**
