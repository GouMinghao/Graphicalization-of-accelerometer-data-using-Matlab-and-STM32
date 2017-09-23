clear
clc
delete(instrfindall);
delete(instrfindall);
s=serial('com11');%this should be changed if the port number is not com11.
set(s,'BaudRate',115200,'DataBits',8,'StopBits',1,'Parity','none','Flowcontrol','none');
s.BytesAvailableFcnMode='byte' ;
s.BytesAvailableFcnCount=200;
s.InputBufferSize = 4096;
s.BytesAvailableFcn = @callback;
fopen(s);

