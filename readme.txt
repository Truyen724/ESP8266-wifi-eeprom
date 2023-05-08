code cho ESP8266
sau khi nạp code và cahyj thì thiết bị sẽ phát wifi
kết nối với mạng wifi đó để thiết lập các thông số qua api

input dạng text(không phải json) chỉ gồm têm wifi, độ dài dưới 150 kí tự, tương tự cho các api còn lại
thiết lập tên wifi: 
method: post
192.168.4.22/set_ssid

thiết lập tên password:
method: post
192.168.4.22/set_password

thiết lập tên server: 
method: post
192.168.4.22/set_server

thiết lập tên id_device:
method: post
192.168.4.22/set_id_device

thiết bị sẽ post lên server thông qua api 
ví dụ: http://172.16.29.209:3001/data
{
	"ssid":"truyen",
	"password":"12345678",
	"id_device":"ID1",
	"server_ip":"http://172.16.29.209:3001/"
}
