
fileID = fopen('Output.csv');

formatSpec = '%f %f %f'; %flyttal

DATA=textscan(fileID,formatSpec,'HeaderLines',1,'delimiter',',','CollectOutput',1); % data till cell

fclose('all');

data = cell2mat(DATA); % cell till matris
tid = data(:,1); 
x = data(:,2) ;
y =  data(:,3);

clf
subplot(1,2,1)
plot(tid,x)
hold on
plot(tid,y,'r')
title('Ours')


%% ode45

%[T,Y] = solver(odefun,tspan,y0,options)

a = 0.4807;
b = 0.02482;
gm = 0.9272;
d = 0.02756;


pred_prey=@(t,x)[a*x(1)-b*x(1)*x(2); d*x(1)*x(2)-gm*x(2)];

tspan = [0 25];

x0 = [34.91; 3.857];

[T,X] = ode45(pred_prey,tspan,x0);

%% plot
subplot(1,2,2)
plot(T,X(:,1))
hold on
plot(T,X(:,2),'r')
title('ode45')

