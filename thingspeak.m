ChannelID = 3084032;        % Your ThingSpeak Channel ID
ReadKey = 'BPDK3A5A28DPS8M8'; % Your ThingSpeak Read API Key
WriteKey = 'TYQZH7SNLKJ0F3PE'; % Your ThingSpeak Write API Key

% Read latest sensor data
pm25   = thingSpeakRead(ChannelID, 'Fields', 1, 'NumPoints', 1, 'ReadKey', ReadKey);
mq2    = thingSpeakRead(ChannelID, 'Fields', 2, 'NumPoints', 1, 'ReadKey', ReadKey);
mq8    = thingSpeakRead(ChannelID, 'Fields', 3, 'NumPoints', 1, 'ReadKey', ReadKey);
mq9    = thingSpeakRead(ChannelID, 'Fields', 4, 'NumPoints', 1, 'ReadKey', ReadKey);
mq135  = thingSpeakRead(ChannelID, 'Fields', 5, 'NumPoints', 1, 'ReadKey', ReadKey);

% Calculate AQI for PM2.5 (example)
aqi_pm25 = calculate_aqi_pm25(pm25);

% You can create similar functions for mq2, mq8, mq9, mq135
% Then combine them (e.g., maximum AQI) for final AQI
final_aqi = aqi_pm25;

% Write AQI to ThingSpeak Field 8
thingSpeakWrite(ChannelID, final_aqi, 'WriteKey', WriteKey, 'Fields', 8);

disp(['Final AQI: ', num2str(final_aqi)]);

% Function for AQI calculation of PM2.5
function aqi = calculate_aqi_pm25(pm25)
    C_low = [0, 12.1, 35.5, 55.5, 150.5, 250.5, 350.5];
    C_high = [12.0, 35.4, 55.4, 150.4, 250.4, 350.4, 500.4];
    I_low = [0, 51, 101, 151, 201, 301, 401];
    I_high = [50, 100, 150, 200, 300, 400, 500];

    aqi = NaN;
    for i = 1:length(C_low)
        if pm25 >= C_low(i) && pm25 <= C_high(i)
            aqi = ((I_high(i) - I_low(i)) / (C_high(i) - C_low(i))) * (pm25 - C_low(i)) + I_low(i);
            break;
        end
    end
end