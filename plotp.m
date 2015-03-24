data1 = dlmread('particle.csv', ',', 9, 0);
data2 = dlmread('particle2.csv', ',', 9, 0);

t1=data1(:,1);
x1=data1(:,3);
y1=data1(:,4);
z1=data1(:,5);

t2=data2(:,1);
x2=data2(:,2);
y2=data2(:,3);
z2=data2(:,4);

plot(sqrt(x1.^2+y1.^2),z1);
hold on
plot(sqrt(x2.^2+y2.^2),z2,'r');
