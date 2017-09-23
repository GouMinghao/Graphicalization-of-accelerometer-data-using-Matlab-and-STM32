function callback(obj,event)
% fprintf('Yes!\n')
a=fgetl(obj);
num = regexp(a, '\s+', 'split');
num = char(num);
num=str2num(num);

mg1=sqrt(num(1:3)'*num(1:3));
mw1=sqrt(num(4:6)'*num(4:6));
mb1=sqrt(num(7:9)'*num(7:9));
mg2=sqrt(num(10:12)'*num(10:12));
mw2=sqrt(num(13:15)'*num(13:15));
mb2=sqrt(num(6:18)'*num(6:18));

gx1=num(1)/mg1;
gy1=num(2)/mg1;
gz1=num(3)/mg1;
wx1=num(4)/mw1;
wy1=num(5)/mw1;
wz1=num(6)/mw1;
bx1=num(7)/mb1;
by1=num(8)/mb1;
bz1=num(9)/mb1;
gx2=num(10)/mg2;
gy2=num(11)/mg2;
gz2=num(12)/mg2;
wx2=num(13)/mw2;
wy2=num(14)/mw2;
wz2=num(15)/mw2;
bx2=num(16)/mb2;
by2=num(17)/mb2;
bz2=num(18)/mb2;

phi=acos(gz1);


plot([0 1],[0 0])
hold on
plot([1,1+cos(phi)],[0,sin(phi)])
hold off
grid on
axis([0 2 0 2])



% plot(num(1),num(2),'+r')
% grid on
% xlabel x
% ylabel num
% axis([0,1,0,1])