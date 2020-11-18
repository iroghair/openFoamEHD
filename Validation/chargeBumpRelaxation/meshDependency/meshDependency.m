RES = [32 64 128 256 512];
a = 0.05;
eps = 2;
K = 1;
timeToLoad=4;

l2 = zeros(1,length(RES));
for i = 1:length(RES)
    fname = ['sample_' num2str(timeToLoad) '_' num2str(RES(i)) '.log'];
    data{i} = importdata(fname);
    pos  = data{i}(:,1);
    rhoe = data{i}(:,2);
    init = exp(-pos.^2/(2*a*a))/(a*sqrt(2*pi));
    exact = init*exp(-K*timeToLoad/eps);
%     figure(i);
%     plot(pos, rhoe,'o');
%     hold on;
%     plot(pos, exact,'-');
    x = abs(exact-rhoe);
    l2(i) = norm(x,2);
    linf(i) = norm(x,inf);
end
figure;
loglog(abs(max(data{i}(:,1)-min(data{i}(:,1))))./(RES),l2,'-x',abs(max(data{i}(:,1)-min(data{i}(:,1))))./(RES),linf,'-o');
xlabel('Mesh size [m]');
ylabel('L_2_,_\infty norm of error vector [-]');
legend('L_2 norm','L_\infty norm')
