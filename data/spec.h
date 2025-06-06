//------------------------------------------------------------------------------
//  (c) 2025 Electronic Arts Inc. All rights reserved.
//------------------------------------------------------------------------------

// C++ structures for F1 25 UDP specification
// Allows easy comparison with previous years to see what has changed

//-----------------------------------------------------------------------------
// Header - 29 bytes
//-----------------------------------------------------------------------------
static const uint32 cs_maxNumCarsInUDPData = 22;
static const uint32 cs_maxParticipantNameLen = 32;
static const uint32 cs_maxTyreStints = 8;
static const uint32 cs_maxNumTyreSets = 13 + 7; // 13 slick and 7 wet weather

// Different packet types
enum PacketId
{
    ePacketIdMotion = 0,              // Contains all motion data for player’s car – only sent while player is in control
    ePacketIdSession = 1,             // Data about the session – track, time left
    ePacketIdLapData = 2,             // Data about all the lap times of cars in the session
    ePacketIdEvent = 3,               // Various notable events that happen during a session
    ePacketIdParticipants = 4,        // List of participants in the session, mostly relevant for multiplayer
    ePacketIdCarSetups = 5,           // Packet detailing car setups for cars in the race
    ePacketIdCarTelemetry = 6,        // Telemetry data for all cars
    ePacketIdCarStatus = 7,           // Status data for all cars
    ePacketIdFinalClassification = 8, // Final classification confirmation at the end of a race
    ePacketIdLobbyInfo = 9,           // Information about players in a multiplayer lobby
    ePacketIdCarDamage = 10,          // Damage status for all cars
    ePacketIdSessionHistory = 11,     // Lap and tyre data for session
    ePacketIdTyreSets = 12,           // Extended tyre set data
    ePacketIdMotionEx = 13,           // Extended motion data for player car
    ePacketIdTimeTrial = 14,          // Time Trial specific data
    ePacketIdLapPositions = 15,       // Lap positions on each lap so a chart can be constructed
    ePacketIdMax
};

struct PacketHeader
{
    uint16 m_packetFormat;           // 2025
    uint8 m_gameYear;                // Game year - last two digits e.g. 25
    uint8 m_gameMajorVersion;        // Game major version - "X.00"
    uint8 m_gameMinorVersion;        // Game minor version - "1.XX"
    uint8 m_packetVersion;           // Version of this packet type
    uint8 m_packetId;                // Identifier for the packet type
    uint64 m_sessionUID;             // Unique identifier for the session
    float m_sessionTime;             // Session timestamp
    uint32 m_frameIdentifier;        // Identifier for the frame the data was retrieved on
    uint32 m_overallFrameIdentifier; // Overall identifier for the frame the data was retrieved on, doesn't go back after flashbacks
    uint8 m_playerCarIndex;          // Index of player's car in the array
    uint8 m_secondaryPlayerCarIndex; // Index of secondary player's car in the array (splitscreen) - 255 if no second player
};

//-----------------------------------------------------------------------------
// Motion - 1349 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Motion data for one car
//-----------------------------------------------------------------------------
struct CarMotionData
{
    float m_worldPositionX;     // World space X position - metres
    float m_worldPositionY;     // World space Y position
    float m_worldPositionZ;     // World space Z position
    float m_worldVelocityX;     // Velocity in world space X - metres/s
    float m_worldVelocityY;     // Velocity in world space Y
    float m_worldVelocityZ;     // Velocity in world space Z
    int16 m_worldForwardDirX;   // World space forward X direction (normalised)
    int16 m_worldForwardDirY;   // World space forward Y direction (normalised)
    int16 m_worldForwardDirZ;   // World space forward Z direction (normalised)
    int16 m_worldRightDirX;     // World space right X direction (normalised)
    int16 m_worldRightDirY;     // World space right Y direction (normalised)
    int16 m_worldRightDirZ;     // World space right Z direction (normalised)
    float m_gForceLateral;      // Lateral G-Force component
    float m_gForceLongitudinal; // Longitudinal G-Force component
    float m_gForceVertical;     // Vertical G-Force component
    float m_yaw;                // Yaw angle in radians
    float m_pitch;              // Pitch angle in radians
    float m_roll;               // Roll angle in radians
};

struct PacketMotionData
{
    PacketHeader m_header; // Header

    // Packet specific data
    CarMotionData m_carMotionData[cs_maxNumCarsInUDPData]; // Data for all cars on track
};

//-----------------------------------------------------------------------------
// Session - 753 bytes
//-----------------------------------------------------------------------------
static const uint32 cs_maxMarshalsZonePerLap = 21;
static const uint32 cs_maxWeatherForecastSamples = 64;
static const uint32 cs_maxSessionsInWeekend = 12;

struct MarshalZone
{
    float m_zoneStart; // Fraction (0..1) of way through the lap the marshal zone starts
    int8 m_zoneFlag;   // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow
};

struct WeatherForecastSample
{
    uint8 m_sessionType;           // 0 = unknown, see appendix
    uint8 m_timeOffset;            // Time in minutes the forecast is for
    uint8 m_weather;               // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    int8 m_trackTemperature;       // Track temp. in degrees celsius
    int8 m_trackTemperatureChange; // Track temp. change - 0 = up, 1 = down, 2 = no change
    int8 m_airTemperature;         // Air temp. in degrees celsius
    int8 m_airTemperatureChange;   // Air temp. change - 0 = up, 1 = down, 2 = no change
    uint8 m_rainPercentage;        // Rain percentage (0-100)
};

struct PacketSessionData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_weather;                                                              // Weather - 0 = clear, 1 = light cloud, 2 = overcast, 3 = light rain, 4 = heavy rain, 5 = storm
    int8 m_trackTemperature;                                                      // Track temp. in degrees celsius
    int8 m_airTemperature;                                                        // Air temp. in degrees celsius
    uint8 m_totalLaps;                                                            // Total number of laps in this race
    uint16 m_trackLength;                                                         // Track length in metres
    uint8 m_sessionType;                                                          // 0 = unknown, see appendix
    int8 m_trackId;                                                               // -1 for unknown, see appendix
    uint8 m_formula;                                                              // Formula, 0 = F1 Modern, 1 = F1 Classic, 2 = F2, 3 = F1 Generic, 4 = Beta, 6 = Esports, 8 = F1 World, 9 = F1 Elimination
    uint16 m_sessionTimeLeft;                                                     // Time left in session in seconds
    uint16 m_sessionDuration;                                                     // Session duration in seconds
    uint8 m_pitSpeedLimit;                                                        // Pit speed limit in kilometres per hour
    uint8 m_gamePaused;                                                           // Whether the game is paused - network game only
    uint8 m_isSpectating;                                                         // Whether the player is spectating
    uint8 m_spectatorCarIndex;                                                    // Index of the car being spectated
    uint8 m_sliProNativeSupport;                                                  // SLI Pro support, 0 = inactive, 1 = active
    uint8 m_numMarshalZones;                                                      // Number of marshal zones to follow
    MarshalZone m_marshalZones[cs_maxMarshalsZonePerLap];                         // List of marshal zones - max 21
    uint8 m_safetyCarStatus;                                                      // 0 = no safety car, 1 = full, 2 = virtual, 3 = formation lap
    uint8 m_networkGame;                                                          // 0 = offline, 1 = online
    uint8 m_numWeatherForecastSamples;                                            // Number of weather samples to follow
    WeatherForecastSample m_weatherForecastSamples[cs_maxWeatherForecastSamples]; // Array of weather forecast samples
    uint8 m_forecastAccuracy;                                                     // 0 = Perfect, 1 = Approximate
    uint8 m_aiDifficulty;                                                         // AI difficulty - 0-110
    uint32 m_seasonLinkIdentifier;                                                // Identifier for season - persists across saves
    uint32 m_weekendLinkIdentifier;                                               // Identifier for weekend - persists across saves
    uint32 m_sessionLinkIdentifier;                                               // Identifier for session - persists across saves
    uint8 m_pitStopWindowIdealLap;                                                // Ideal lap to pit on for current strategy (player)
    uint8 m_pitStopWindowLatestLap;                                               // Latest lap to pit on for current strategy (player)
    uint8 m_pitStopRejoinPosition;                                                // Predicted position to rejoin at (player)
    uint8 m_steeringAssist;                                                       // 0 = off, 1 = on
    uint8 m_brakingAssist;                                                        // 0 = off, 1 = low, 2 = medium, 3 = high
    uint8 m_gearboxAssist;                                                        // 1 = manual, 2 = manual & suggested gear, 3 = auto
    uint8 m_pitAssist;                                                            // 0 = off, 1 = on
    uint8 m_pitReleaseAssist;                                                     // 0 = off, 1 = on
    uint8 m_ERSAssist;                                                            // 0 = off, 1 = on
    uint8 m_DRSAssist;                                                            // 0 = off, 1 = on
    uint8 m_dynamicRacingLine;                                                    // 0 = off, 1 = corners only, 2 = full
    uint8 m_dynamicRacingLineType;                                                // 0 = 2D, 1 = 3D
    uint8 m_gameMode;                                                             // Game mode id - see appendix
    uint8 m_ruleSet;                                                              // Ruleset - see appendix
    uint32 m_timeOfDay;                                                           // Local time of day - minutes since midnight
    uint8 m_sessionLength;                                                        // 0 = None, 2 = Very Short, 3 = Short, 4 = Medium, 5 = Medium Long, 6 = Long, 7 = Full
    uint8 m_speedUnitsLeadPlayer;                                                 // 0 = MPH, 1 = KPH
    uint8 m_temperatureUnitsLeadPlayer;                                           // 0 = Celsius, 1 = Fahrenheit
    uint8 m_speedUnitsSecondaryPlayer;                                            // 0 = MPH, 1 = KPH
    uint8 m_temperatureUnitsSecondaryPlayer;                                      // 0 = Celsius, 1 = Fahrenheit
    uint8 m_numSafetyCarPeriods;                                                  // Number of safety cars called during session
    uint8 m_numVirtualSafetyCarPeriods;                                           // Number of virtual safety cars called during session
    uint8 m_numRedFlagPeriods;                                                    // Number of red flags called during session
    uint8 m_equalCarPerformance;                                                  // 0 = Off, 1 = On
    uint8 m_recoveryMode;                                                         // 0 = None, 1 = Flashbacks, 2 = Auto-recovery
    uint8 m_flashbackLimit;                                                       // 0 = Low, 1 = Medium, 2 = High, 3 = Unlimited
    uint8 m_surfaceType;                                                          // 0 = Simplified, 1 = Realistic
    uint8 m_lowFuelMode;                                                          // 0 = Easy, 1 = Hard
    uint8 m_raceStarts;                                                           // 0 = Manual, 1 = Assisted
    uint8 m_tyreTemperature;                                                      // 0 = Surface only, 1 = Surface & Carcass
    uint8 m_pitLaneTyreSim;                                                       // 0 = On, 1 = Off
    uint8 m_carDamage;                                                            // 0 = Off, 1 = Reduced, 2 = Standard, 3 = Simulation
    uint8 m_carDamageRate;                                                        // 0 = Reduced, 1 = Standard, 2 = Simulation
    uint8 m_collisions;                                                           // 0 = Off, 1 = Player-to-Player Off, 2 = On
    uint8 m_collisionsOffForFirstLapOnly;                                         // 0 = Disabled, 1 = Enabled
    uint8 m_mpUnsafePitRelease;                                                   // 0 = On, 1 = Off (Multiplayer)
    uint8 m_mpOffForGriefing;                                                     // 0 = Disabled, 1 = Enabled (Multiplayer)
    uint8 m_cornerCuttingStringency;                                              // 0 = Regular, 1 = Strict
    uint8 m_parcFermeRules;                                                       // 0 = Off, 1 = On
    uint8 m_pitStopExperience;                                                    // 0 = Automatic, 1 = Broadcast, 2 = Immersive
    uint8 m_safetyCar;                                                            // 0 = Off, 1 = Reduced, 2 = Standard, 3 = Increased
    uint8 m_safetyCarExperience;                                                  // 0 = Broadcast, 1 = Immersive
    uint8 m_formationLap;                                                         // 0 = Off, 1 = On
    uint8 m_formationLapExperience;                                               // 0 = Broadcast, 1 = Immersive
    uint8 m_redFlags;                                                             // 0 = Off, 1 = Reduced, 2 = Standard, 3 = Increased
    uint8 m_affectsLicenceLevelSolo;                                              // 0 = Off, 1 = On
    uint8 m_affectsLicenceLevelMP;                                                // 0 = Off, 1 = On
    uint8 m_numSessionsInWeekend;                                                 // Number of session in following array
    uint8 m_weekendStructure[cs_maxSessionsInWeekend];                            // List of session types to show weekend structure - see appendix for types
    float m_sector2LapDistanceStart;                                              // Distance in m around track where sector 2 starts
    float m_sector3LapDistanceStart;                                              // Distance in m around track where sector 3 starts
};

//-----------------------------------------------------------------------------
// Lap - 1285 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Lap data about one car
//-----------------------------------------------------------------------------
struct LapData
{
    uint32 m_lastLapTimeInMS;             // Last lap time in milliseconds
    uint32 m_currentLapTimeInMS;          // Current time around the lap in milliseconds
    uint16 m_sector1TimeMSPart;           // Sector 1 time milliseconds part
    uint8 m_sector1TimeMinutesPart;       // Sector 1 whole minute part
    uint16 m_sector2TimeMSPart;           // Sector 2 time milliseconds part
    uint8 m_sector2TimeMinutesPart;       // Sector 2 whole minute part
    uint16 m_deltaToCarInFrontMSPart;     // Time delta to car in front milliseconds part
    uint8 m_deltaToCarInFrontMinutesPart; // Time delta to car in front whole minute part
    uint16 m_deltaToRaceLeaderMSPart;     // Time delta to race leader milliseconds part
    uint8 m_deltaToRaceLeaderMinutesPart; // Time delta to race leader whole minute part
    float m_lapDistance;                  // Distance vehicle is around current lap in metres – could be negative if line hasn’t been crossed yet
    float m_totalDistance;                // Total distance travelled in session in metres – could be negative if line hasn’t been crossed yet
    float m_safetyCarDelta;               // Delta in seconds for safety car
    uint8 m_carPosition;                  // Car race position
    uint8 m_currentLapNum;                // Current lap number
    uint8 m_pitStatus;                    // 0 = none, 1 = pitting, 2 = in pit area
    uint8 m_numPitStops;                  // Number of pit stops taken in this race
    uint8 m_sector;                       // 0 = sector1, 1 = sector2, 2 = sector3
    uint8 m_currentLapInvalid;            // Current lap invalid - 0 = valid, 1 = invalid
    uint8 m_penalties;                    // Accumulated time penalties in seconds to be added
    uint8 m_totalWarnings;                // Accumulated number of warnings issued
    uint8 m_cornerCuttingWarnings;        // Accumulated number of corner cutting warnings issued
    uint8 m_numUnservedDriveThroughPens;  // Num drive through pens left to serve
    uint8 m_numUnservedStopGoPens;        // Num stop go pens left to serve
    uint8 m_gridPosition;                 // Grid position the vehicle started the race in
    uint8 m_driverStatus;                 // Status of driver - 0 = in garage, 1 = flying lap, 2 = in lap, 3 = out lap, 4 = on track
    uint8 m_resultStatus;                 // Result status - 0 = invalid, 1 = inactive, 2 = active, 3 = finished, 4 = didnotfinish, 5 = disqualified, 6 = not classified, 7 = retired
    uint8 m_pitLaneTimerActive;           // Pit lane timing, 0 = inactive, 1 = active
    uint16 m_pitLaneTimeInLaneInMS;       // If active, the current time spent in the pit lane in ms
    uint16 m_pitStopTimerInMS;            // Time of the actual pit stop in ms
    uint8 m_pitStopShouldServePen;        // Whether the car should serve a penalty at this stop
    float m_speedTrapFastestSpeed;        // Fastest speed through speed trap for this car in kmph
    uint8 m_speedTrapFastestLap;          // Lap no the fastest speed was achieved, 255 = not set
};

struct PacketLapData
{
    PacketHeader m_header; // Header

    // Packet specific data
    LapData m_lapData[cs_maxNumCarsInUDPData]; // Lap data for all cars on track

    uint8 m_timeTrialPBCarIdx;    // Index of Personal Best car in time trial (255 if invalid)
    uint8 m_timeTrialRivalCarIdx; // Index of Rival car in time trial (255 if invalid)
};

//-----------------------------------------------------------------------------
// Event - 45 bytes
//-----------------------------------------------------------------------------

static const uint cs_eventStringCodeLen = 4;

// The event details packet is different for each type of event.
// Make sure only the correct type is interpreted.
union EventDataDetails
{
    struct
    {
        uint8 vehicleIdx; // Vehicle index of car achieving fastest lap
        float lapTime;    // Lap time is in seconds
    } FastestLap;

    struct
    {
        uint8 vehicleIdx; // Vehicle index of car retiring
        uint8 reason;     // Result reason - 0 = invalid, 1 = retired, 2 = finished, 3 = terminal damage, 4 = inactive, 5 = not enough laps completed, 6 = black flagged
                          // 7 = red flagged, 8 = mechanical failure, 9 = session skipped, 10 = session simulated
    } Retirement;

    struct
    {
        uint8 reason; // 0 = Wet track, 1 = Safety car deployed, 2 = Red flag, 3 = Min lap not reached
    } DRSDisabled;

    struct
    {
        uint8 vehicleIdx; // Vehicle index of team mate
    } TeamMateInPits;

    struct
    {
        uint8 vehicleIdx; // Vehicle index of the race winner
    } RaceWinner;

    struct
    {
        uint8 penaltyType;      // Penalty type – see Appendices
        uint8 infringementType; // Infringement type – see Appendices
        uint8 vehicleIdx;       // Vehicle index of the car the penalty is applied to
        uint8 otherVehicleIdx;  // Vehicle index of the other car involved
        uint8 time;             // Time gained, or time spent doing action in seconds
        uint8 lapNum;           // Lap the penalty occurred on
        uint8 placesGained;     // Number of places gained by this
    } Penalty;

    struct
    {
        uint8 vehicleIdx;                 // Vehicle index of the vehicle triggering speed trap
        float speed;                      // Top speed achieved in kilometres per hour
        uint8 isOverallFastestInSession;  // Overall fastest speed in session = 1, otherwise 0
        uint8 isDriverFastestInSession;   // Fastest speed for driver in session = 1, otherwise 0
        uint8 fastestVehicleIdxInSession; // Vehicle index of the vehicle that is the fastest in this session
        float fastestSpeedInSession;      // Speed of the vehicle that is the fastest in this session
    } SpeedTrap;

    struct
    {
        uint8 numLights; // Number of lights showing
    } StartLights;

    struct
    {
        uint8 vehicleIdx; // Vehicle index of the vehicle serving drive through
    } DriveThroughPenaltyServed;

    struct
    {
        uint8 vehicleIdx; // Vehicle index of the vehicle serving stop go
        float stopTime;   // Time spent serving stop go in seconds
    } StopGoPenaltyServed;

    struct
    {
        uint32 flashbackFrameIdentifier; // Frame identifier flashed back to
        float flashbackSessionTime;      // Session time flashed back to
    } Flashback;

    struct
    {
        uint32 buttonStatus; // Bit flags specifying which buttons are being pressed currently - see appendices
    } Buttons;

    struct
    {
        uint8 overtakingVehicleIdx;     // Vehicle index of the vehicle overtaking
        uint8 beingOvertakenVehicleIdx; // Vehicle index of the vehicle being overtaken
    } Overtake;

    struct
    {
        uint8 safetyCarType; // 0 = No Safety Car, 1 = Full Safety Car, 2 = Virtual Safety Car, 3 = Formation Lap Safety Car
        uint8 eventType;     // 0 = Deployed, 1 = Returning, 2 = Returned, 3 = Resume Race
    } SafetyCar;

    struct
    {
        uint8 vehicle1Idx; // Vehicle index of the first vehicle involved in the collision
        uint8 vehicle2Idx; // Vehicle index of the second vehicle involved in the collision
    } Collision;
};

struct PacketEventData
{
    // Valid event strings
    static const NEchar *cs_sessionStartedEventCode;     // "SSTA"
    static const NEchar *cs_sessionEndedEventCode;       // "SEND"
    static const NEchar *cs_fastestLapEventCode;         // "FTLP"
    static const NEchar *cs_retirementEventCode;         // "RTMT"
    static const NEchar *cs_drsEnabledEventCode;         // "DRSE"
    static const NEchar *cs_drsDisabledEventCode;        // "DRSD"
    static const NEchar *cs_teamMateInPitsEventCode;     // "TMPT"
    static const NEchar *cs_chequeredFlagEventCode;      // "CHQF"
    static const NEchar *cs_raceWinnerEventCode;         // "RCWN"
    static const NEchar *cs_penaltyEventCode;            // "PENA"
    static const NEchar *cs_speedTrapEventCode;          // "SPTP"
    static const NEchar *cs_startLightsEventCode;        // "STLG"
    static const NEchar *cs_lightsOutEventCode;          // "LGOT"
    static const NEchar *cs_driveThroughServedEventCode; // "DTSV"
    static const NEchar *cs_stopGoServedEventCode;       // "SGSV"
    static const NEchar *cs_flashbackEventCode;          // "FLBK"
    static const NEchar *cs_buttonStatusEventCode;       // "BUTN"
    static const NEchar *cs_redFlagEventCode;            // "RDFL"
    static const NEchar *cs_overtakeEventCode;           // "OVTK"
    static const NEchar *cs_safetyCarEventCode;          // "SCAR"
    static const NEchar *cs_collisionEventCode;          // "COLL"

    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_eventStringCode[cs_eventStringCodeLen]; // Event string code
    EventDataDetails m_eventDetails;                // Event details - should be interpreted differently for each type
};

//-----------------------------------------------------------------------------
// Participants - 1284 bytes
//-----------------------------------------------------------------------------

// RGB value of a colour
struct LiveryColour
{
    uint8 red;
    uint8 green;
    uint8 blue;
};

//-----------------------------------------------------------------------------
// Data about one participant
//-----------------------------------------------------------------------------
struct ParticipantData
{
    uint8 m_aiControlled;                  // Whether the vehicle is AI (1) or Human (0) controlled
    uint8 m_driverId;                      // Driver id - see appendix, 255 if network human
    uint8 m_networkId;                     // Network id - unique identifier for network players
    uint8 m_teamId;                        // Team id - see appendix
    uint8 m_myTeam;                        // My team flag - 1 = My Team, 0 = otherwise
    uint8 m_raceNumber;                    // Race number of the car
    uint8 m_nationality;                   // Nationality of the driver
    char m_name[cs_maxParticipantNameLen]; // Name of participant in UTF-8 format – null terminated
                                           // Will be truncated with ... (U+2026) if too long
    uint8 m_yourTelemetry;                 // The player's UDP setting, 0 = restricted, 1 = public
    uint8 m_showOnlineNames;               // The player's show online names setting, 0 = off, 1 = on
    uint16 m_techLevel;                    // F1 World tech level
    uint8 m_platform;                      // 1 = Steam, 3 = PlayStation, 4 = Xbox, 6 = Origin, 255 = unknown
    uint8 m_numColours;                    // Number of colours valid for this car
    LiveryColour m_liveryColours[4];       // Colours for the car
};

struct PacketParticipantsData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_numActiveCars; // Number of active cars in the data - should match number of cars on HUD
    ParticipantData m_participants[cs_maxNumCarsInUDPData];
};

//-----------------------------------------------------------------------------
// Car Setups - 1133 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Data about one car setup
//-----------------------------------------------------------------------------
struct CarSetupData
{
    uint8 m_frontWing;              // Front wing aero
    uint8 m_rearWing;               // Rear wing aero
    uint8 m_onThrottle;             // Differential adjustment on throttle (percentage)
    uint8 m_offThrottle;            // Differential adjustment off throttle (percentage)
    float m_frontCamber;            // Front camber angle (suspension geometry)
    float m_rearCamber;             // Rear camber angle (suspension geometry)
    float m_frontToe;               // Front toe angle (suspension geometry)
    float m_rearToe;                // Rear toe angle (suspension geometry)
    uint8 m_frontSuspension;        // Front suspension
    uint8 m_rearSuspension;         // Rear suspension
    uint8 m_frontAntiRollBar;       // Front anti-roll bar
    uint8 m_rearAntiRollBar;        // Front anti-roll bar
    uint8 m_frontSuspensionHeight;  // Front ride height
    uint8 m_rearSuspensionHeight;   // Rear ride height
    uint8 m_brakePressure;          // Brake pressure (percentage)
    uint8 m_brakeBias;              // Brake bias (percentage)
    uint8 m_engineBraking;          // Engine braking (percentage)
    float m_rearLeftTyrePressure;   // Rear left tyre pressure (PSI)
    float m_rearRightTyrePressure;  // Rear right tyre pressure (PSI)
    float m_frontLeftTyrePressure;  // Front left tyre pressure (PSI)
    float m_frontRightTyrePressure; // Front right tyre pressure (PSI)
    uint8 m_ballast;                // Ballast
    float m_fuelLoad;               // Fuel load
};

struct PacketCarSetupData
{
    PacketHeader m_header; // Header

    // Packet specific data
    CarSetupData m_carSetupData[cs_maxNumCarsInUDPData];

    float m_nextFrontWingValue; // Value of front wing after next pit stop - player only
};

//-----------------------------------------------------------------------------
// Car Telemetry - 1352 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Telemetry data for one car
//-----------------------------------------------------------------------------
struct CarTelemetryData
{
    uint16 m_speed;                     // Speed of car in kilometres per hour
    float m_throttle;                   // Amount of throttle applied (0.0 to 1.0)
    float m_steer;                      // Steering (-1.0 (full lock left) to 1.0 (full lock right))
    float m_brake;                      // Amount of brake applied (0.0 to 1.0)
    uint8 m_clutch;                     // Amount of clutch applied (0 to 100)
    int8 m_gear;                        // Gear selected (1-8, N=0, R=-1)
    uint16 m_engineRPM;                 // Engine RPM
    uint8 m_drs;                        // 0 = off, 1 = on
    uint8 m_revLightsPercent;           // Rev lights indicator (percentage)
    uint16 m_revLightsBitValue;         // Rev lights (bit 0 = leftmost LED, bit 14 = rightmost LED)
    uint16 m_brakesTemperature[4];      // Brakes temperature (celsius)
    uint8 m_tyresSurfaceTemperature[4]; // Tyres surface temperature (celsius)
    uint8 m_tyresInnerTemperature[4];   // Tyres inner temperature (celsius)
    uint16 m_engineTemperature;         // Engine temperature (celsius)
    float m_tyresPressure[4];           // Tyre pressure (PSI)
    uint8 m_surfaceType[4];             // Driving surface, see appendices
};

struct PacketCarTelemetryData
{
    PacketHeader m_header; // Header

    // Packet specific data
    CarTelemetryData m_carTelemetryData[cs_maxNumCarsInUDPData]; // data for all cars on track

    uint8 m_mfdPanelIndex;                // Index of MFD panel open - 255 = MFD closed
                                          // Single player, race – 0 = Car setup, 1 = Pits
                                          // 2 = Damage, 3 =  Engine, 4 = Temperatures
                                          // May vary depending on game mode
    uint8 m_mfdPanelIndexSecondaryPlayer; // See above
    int8 m_suggestedGear;                 // Suggested gear for the player (1-8), 0 if no gear suggested
};

//-----------------------------------------------------------------------------
// Car Status - 1239 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Car status data for one car
//-----------------------------------------------------------------------------
struct CarStatusData
{
    uint8 m_tractionControl;         // Traction control - 0 = off, 1 = medium, 2 = full
    uint8 m_antiLockBrakes;          // 0 (off) - 1 (on)
    uint8 m_fuelMix;                 // Fuel mix - 0 = lean, 1 = standard, 2 = rich, 3 = max
    uint8 m_frontBrakeBias;          // Front brake bias (percentage)
    uint8 m_pitLimiterStatus;        // Pit limiter status - 0 = off, 1 = on
    float m_fuelInTank;              // Current fuel mass
    float m_fuelCapacity;            // Fuel capacity
    float m_fuelRemainingLaps;       // Fuel remaining in terms of laps (value on MFD)
    uint16 m_maxRPM;                 // Cars max RPM, point of rev limiter
    uint16 m_idleRPM;                // Cars idle RPM
    uint8 m_maxGears;                // Maximum number of gears
    uint8 m_drsAllowed;              // 0 = not allowed, 1 = allowed
    uint16 m_drsActivationDistance;  // 0 = DRS not available, non-zero - DRS will be available in [X] metres
    uint8 m_actualTyreCompound;      // F1 Modern - 16 = C5, 17 = C4, 18 = C3, 19 = C2, 20 = C1, 21 = C0, 22 = C6, 7 = inter, 8 = wet
                                     // F1 Classic - 9 = dry, 10 = wet
                                     // F2 – 11 = super soft, 12 = soft, 13 = medium, 14 = hard, 15 = wet
    uint8 m_visualTyreCompound;      // F1 visual (can be different from actual compound)
                                     // 16 = soft, 17 = medium, 18 = hard, 7 = inter, 8 = wet
                                     // F1 Classic – same as above
                                     // F2 ‘20, 15 = wet, 19 – super soft, 20 = soft, 21 = medium, 22 = hard
    uint8 m_tyresAgeLaps;            // Age in laps of the current set of tyres
    int8 m_vehicleFIAFlags;          // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow
    float m_enginePowerICE;          // Engine power output of ICE (W)
    float m_enginePowerMGUK;         // Engine power output of MGU-K (W)
    float m_ersStoreEnergy;          // ERS energy store in Joules
    uint8 m_ersDeployMode;           // ERS deployment mode, 0 = none, 1 = medium, 2 = hotlap, 3 = overtake
    float m_ersHarvestedThisLapMGUK; // ERS energy harvested this lap by MGU-K
    float m_ersHarvestedThisLapMGUH; // ERS energy harvested this lap by MGU-H
    float m_ersDeployedThisLap;      // ERS energy deployed this lap
    uint8 m_networkPaused;           // Whether the car is paused in a network game
};

struct PacketCarStatusData
{
    PacketHeader m_header; // Header

    // Packet specific data
    CarStatusData m_carStatusData[cs_maxNumCarsInUDPData]; // data for all cars on track
};

//-----------------------------------------------------------------------------
// Final Classification - 1042 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Data about one participant's final results
//-----------------------------------------------------------------------------
struct FinalClassificationData
{
    uint8 m_position;                            // Finishing position
    uint8 m_numLaps;                             // Number of laps completed
    uint8 m_gridPosition;                        // Grid position of the car
    uint8 m_points;                              // Number of points scored
    uint8 m_numPitStops;                         // Number of pit stops made
    uint8 m_resultStatus;                        // Result status - 0 = invalid, 1 = inactive, 2 = active, 3 = finished, 4 = didnotfinish, 5 = disqualified, 6 = not classified, 7 = retired
    uint8 m_resultReason;                        // Result reason - 0 = invalid, 1 = retired, 2 = finished, 3 = terminal damage, 4 = inactive, 5 = not enough laps completed, 6 = black flagged
                                                 // 7 = red flagged, 8 = mechanical failure, 9 = session skipped, 10 = session simulated
    uint32 m_bestLapTimeInMS;                    // Best lap time of the session in milliseconds
    double m_totalRaceTime;                      // Total race time in seconds without penalties
    uint8 m_penaltiesTime;                       // Total penalties accumulated in seconds
    uint8 m_numPenalties;                        // Number of penalties applied to this driver
    uint8 m_numTyreStints;                       // Number of tyres stints up to maximum
    uint8 m_tyreStintsActual[cs_maxTyreStints];  // Actual tyres used by this driver
    uint8 m_tyreStintsVisual[cs_maxTyreStints];  // Visual tyres used by this driver
    uint8 m_tyreStintsEndLaps[cs_maxTyreStints]; // The lap number stints end on
};

struct PacketFinalClassificationData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_numCars; // Number of cars in the final classification
    FinalClassificationData m_classificationData[cs_maxNumCarsInUDPData];
};

//-----------------------------------------------------------------------------
// Lobby Info - 954 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Data about one participant
//-----------------------------------------------------------------------------
struct LobbyInfoData
{
    uint8 m_aiControlled;                  // Whether the vehicle is AI (1) or Human (0) controlled
    uint8 m_teamId;                        // Team id - see appendix (255 if no team currently selected)
    uint8 m_nationality;                   // Nationality of the driver
    uint8 m_platform;                      // 1 = Steam, 3 = PlayStation, 4 = Xbox, 6 = Origin, 255 = unknown
    char m_name[cs_maxParticipantNameLen]; // Name of participant in UTF-8 format – null terminated
                                           // Will be truncated with ... (U+2026) if too long
    uint8 m_carNumber;                     // Car number of the player
    uint8 m_yourTelemetry;                 // The player's UDP setting, 0 = restricted, 1 = public
    uint8 m_showOnlineNames;               // The player's show online names setting, 0 = off, 1 = on
    uint16 m_techLevel;                    // F1 World tech level
    uint8 m_readyStatus;                   // 0 = not ready, 1 = ready, 2 = spectating
};

struct PacketLobbyInfoData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_numPlayers; // Number of players in the lobby data
    LobbyInfoData m_lobbyPlayers[cs_maxNumCarsInUDPData];
};

//-----------------------------------------------------------------------------
// Car Damage - 1041 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Car damage data for one car
//-----------------------------------------------------------------------------
struct CarDamageData
{
    float m_tyresWear[4];         // Tyre wear (percentage)
    uint8 m_tyresDamage[4];       // Tyre damage (percentage)
    uint8 m_brakesDamage[4];      // Brakes damage (percentage)
    uint8 m_tyreBlisters[4];      // Tyre blisters value (percentage)
    uint8 m_frontLeftWingDamage;  // Front left wing damage (percentage)
    uint8 m_frontRightWingDamage; // Front right wing damage (percentage)
    uint8 m_rearWingDamage;       // Rear wing damage (percentage)
    uint8 m_floorDamage;          // Floor damage (percentage)
    uint8 m_diffuserDamage;       // Diffuser damage (percentage)
    uint8 m_sidepodDamage;        // Sidepod damage (percentage)
    uint8 m_drsFault;             // Indicator for DRS fault, 0 = OK, 1 = fault
    uint8 m_ersFault;             // Indicator for ERS fault, 0 = OK, 1 = fault
    uint8 m_gearBoxDamage;        // Gear box damage (percentage)
    uint8 m_engineDamage;         // Engine damage (percentage)
    uint8 m_engineMGUHWear;       // Engine wear MGU-H (percentage)
    uint8 m_engineESWear;         // Engine wear ES (percentage)
    uint8 m_engineCEWear;         // Engine wear CE (percentage)
    uint8 m_engineICEWear;        // Engine wear ICE (percentage)
    uint8 m_engineMGUKWear;       // Engine wear MGU-K (percentage)
    uint8 m_engineTCWear;         // Engine wear TC (percentage)
    uint8 m_engineBlown;          // Engine blown, 0 = OK, 1 = fault
    uint8 m_engineSeized;         // Engine seized, 0 = OK, 1 = fault
};

struct PacketCarDamageData
{
    PacketHeader m_header; // Header

    // Packet specific data
    CarDamageData m_carDamageData[cs_maxNumCarsInUDPData]; // data for all cars on track
};

//-----------------------------------------------------------------------------
// Session History - 1460 bytes
//-----------------------------------------------------------------------------

static const uint cs_maxNumLapsInHistory = 100;

struct LapHistoryData
{
    uint32 m_lapTimeInMS;           // Lap time in milliseconds
    uint16 m_sector1TimeMSPart;     // Sector 1 milliseconds part
    uint8 m_sector1TimeMinutesPart; // Sector 1 whole minute part
    uint16 m_sector2TimeMSPart;     // Sector 2 time milliseconds part
    uint8 m_sector2TimeMinutesPart; // Sector 2 whole minute part
    uint16 m_sector3TimeMSPart;     // Sector 3 time milliseconds part
    uint8 m_sector3TimeMinutesPart; // Sector 3 whole minute part
    uint8 m_lapValidBitFlags;       // 0x01 bit set-lap valid,      0x02 bit set-sector 1 valid
                                    // 0x04 bit set-sector 2 valid, 0x08 bit set-sector 3 valid
};

struct TyreStintHistoryData
{
    uint8 m_endLap;             // Lap the tyre usage ends on (255 if current tyre)
    uint8 m_tyreActualCompound; // Actual tyres used by this driver
    uint8 m_tyreVisualCompound; // Visual tyres used by this driver
};

struct PacketSessionHistoryData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_carIdx;        // Index of the car this lap data relates to
    uint8 m_numLaps;       // Num laps in the data (including current partial lap)
    uint8 m_numTyreStints; // Number of tyre stints in the data

    uint8 m_bestLapTimeLapNum; // Lap the best lap time was achieved on
    uint8 m_bestSector1LapNum; // Lap the best Sector 1 time was achieved on
    uint8 m_bestSector2LapNum; // Lap the best Sector 2 time was achieved on
    uint8 m_bestSector3LapNum; // Lap the best Sector 3 time was achieved on

    LapHistoryData m_lapHistoryData[cs_maxNumLapsInHistory]; // 100 laps of data max
    TyreStintHistoryData m_tyreStintsHistoryData[cs_maxTyreStints];
};

//-----------------------------------------------------------------------------
// Tyre Sets - 231 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Data about one tyre set
//-----------------------------------------------------------------------------
struct TyreSetData
{
    uint8 m_actualTyreCompound; // Actual tyre compound used
    uint8 m_visualTyreCompound; // Visual tyre compound used
    uint8 m_wear;               // Tyre wear (percentage)
    uint8 m_available;          // Whether this set is currently available
    uint8 m_recommendedSession; // Recommended session for tyre set, see appendix
    uint8 m_lifeSpan;           // Laps left in this tyre set
    uint8 m_usableLife;         // Max number of laps recommended for this compound
    int16 m_lapDeltaTime;       // Lap delta time in milliseconds compared to fitted set
    uint8 m_fitted;             // Whether the set is fitted or not
};

struct PacketTyreSetsData
{
    PacketHeader m_header; // Header

    uint8 m_carIdx; // Index of the car this data relates to

    // Packet specific data
    TyreSetData m_tyreSetData[cs_maxNumTyreSets]; // 13 (dry) + 7 (wet)

    uint8 m_fittedIdx; // Index into array of fitted tyre
};

//-----------------------------------------------------------------------------
// Motion Ex - 273 bytes
//-----------------------------------------------------------------------------

struct PacketMotionExData
{
    PacketHeader m_header; // Header

    // Extra player car ONLY data
    float m_suspensionPosition[4];     // Note: All wheel arrays have the following order:
    float m_suspensionVelocity[4];     // RL, RR, FL, FR
    float m_suspensionAcceleration[4]; // RL, RR, FL, FR
    float m_wheelSpeed[4];             // Speed of each wheel
    float m_wheelSlipRatio[4];         // Slip ratio for each wheel
    float m_wheelSlipAngle[4];         // Slip angles for each wheel
    float m_wheelLatForce[4];          // Lateral forces for each wheel
    float m_wheelLongForce[4];         // Longitudinal forces for each wheel
    float m_heightOfCOGAboveGround;    // Height of centre of gravity above ground
    float m_localVelocityX;            // Velocity in local space X - metres/s
    float m_localVelocityY;            // Velocity in local space Y
    float m_localVelocityZ;            // Velocity in local space Z
    float m_angularVelocityX;          // Angular velocity x-component - radians/s
    float m_angularVelocityY;          // Angular velocity y-component
    float m_angularVelocityZ;          // Angular velocity z-component
    float m_angularAccelerationX;      // Angular acceleration x-component - radians/s/s
    float m_angularAccelerationY;      // Angular acceleration y-component
    float m_angularAccelerationZ;      // Angular acceleration z-component
    float m_frontWheelsAngle;          // Current front wheels angle in radians
    float m_wheelVertForce[4];         // Vertical forces for each wheel
    float m_frontAeroHeight;           // Front plank edge height above road surface
    float m_rearAeroHeight;            // Rear plank edge height above road surface
    float m_frontRollAngle;            // Roll angle of the front suspension
    float m_rearRollAngle;             // Roll angle of the rear suspension
    float m_chassisYaw;                // Yaw angle of the chassis relative to the direction of motion - radians
    float m_chassisPitch;              // Pitch angle of the chassis relative to the direction of motion - radians
    float m_wheelCamber[4];            // Camber of each wheel in radians
    float m_wheelCamberGain[4];        // Camber gain for each wheel in radians, difference between active camber and dynamic camber
};

//-----------------------------------------------------------------------------
// Time Trial - 101 bytes
//-----------------------------------------------------------------------------

struct TimeTrialDataSet
{
    uint8 m_carIdx;              // Index of the car this data relates to
    uint8 m_teamId;              // Team id - see appendix
    uint m_lapTimeInMS;          // Lap time in milliseconds
    uint m_sector1TimeInMS;      // Sector 1 time in milliseconds
    uint m_sector2TimeInMS;      // Sector 2 time in milliseconds
    uint m_sector3TimeInMS;      // Sector 3 time in milliseconds
    uint8 m_tractionControl;     // 0 = assist off, 1 = assist on
    uint8 m_gearboxAssist;       // 0 = assist off, 1 = assist on
    uint8 m_antiLockBrakes;      // 0 = assist off, 1 = assist on
    uint8 m_equalCarPerformance; // 0 = Realistic, 1 = Equal
    uint8 m_customSetup;         // 0 = No, 1 = Yes
    uint8 m_valid;               // 0 = invalid, 1 = valid
};

struct PacketTimeTrialData
{
    PacketHeader m_header; // Header

    TimeTrialDataSet m_playerSessionBestDataSet; // Player session best data set
    TimeTrialDataSet m_personalBestDataSet;      // Personal best data set
    TimeTrialDataSet m_rivalDataSet;             // Rival data set
};

//-----------------------------------------------------------------------------
// Lap Positions - 1131 bytes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Packet to send UDP data about the lap positions in a session. It details
// the positions of all the drivers at the start of each lap
//-----------------------------------------------------------------------------
static const uint cs_maxNumLapsInLapPositionsHistoryPacket = 50;

struct PacketLapPositionsData
{
    PacketHeader m_header; // Header

    // Packet specific data
    uint8 m_numLaps;  // Number of laps in the data
    uint8 m_lapStart; // Index of the lap where the data starts, 0 indexed

    // Array holding the position of the car in a given lap, 0 if no record
    uint8 m_positionForVehicleIdx[cs_maxNumLapsInLapPositionsHistoryPacket][cs_maxNumCarsInUDPData];
};

//////////////////////////////// END OF FILE ////////////////////////////////
