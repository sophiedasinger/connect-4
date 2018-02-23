const {GetNextMove} = require('./build/Release/addon');

console.log(GetNextMove(""));


// Connect 4 Client
//

var Pusher = require('pusher-client');
var request = require('request');

var API_KEY = 'ff13411cc742ac00cff0';
var API_URL = 'http://games-1166.draftkingstest.com:4044';

// player id
var PLAYER_ID = 'PONSi.scheme'; // YOUR TEAM NAME HERE
var MATCH_ID = 'ponsi'; // MATCH NAME HERE

// pusher channel
var PLAYER_CHANNEL = 'connect4.player.' + PLAYER_ID;

// pusher event names
var YOUR_TURN_EVENT = "YourTurn";
var GAME_CREATED_EVENT = "GameCreated";
var GAME_STARTED_EVENT = "GameStarted";
var GAME_OVER_EVENT = "GameOver";

// pusher setup
var pusher = new Pusher(API_KEY, { cluster: "us2" });
var channel = pusher.subscribe(PLAYER_CHANNEL);

var rows = 0, columns = 0;
var board;

console.log('Press any key to register for match');
process.stdin.once('data', function () {
  registerForMatch(function(data){
      console.log(data);
  });
});

channel.bind(GAME_CREATED_EVENT, function(data) { // game created
    console.log("EVENT: new game created");
});

channel.bind(GAME_STARTED_EVENT, function(data) { // game started
    rows = data.rows;
    columns = data.columns;
    board = data.board;
    console.log("EVENT: new game started, board is " + rows + "x" + columns);
    printBoard();
});

channel.bind(YOUR_TURN_EVENT, function(data) { // turn logic
    console.log("EVENT: YourTurn");
    board = data.board;
    printBoard();
    var cols = whatColumnsHaveSpace();
    var pickedColumn = pickColumn(cols);
    sendMove(pickedColumn, function(data) {
        console.log("Sent move for column: " + pickedColumn + "\n");
    });
});

channel.bind(GAME_OVER_EVENT, function(data) { // game over
    console.log("EVENT: GameOver");
});

var pickColumn = function(colArray) {
    // CALL C++ CODE HERE
};


var printBoard = function() {
    for (var i = rows - 1; i >= 0; i--) {
        console.log(board[i]);
    }
};

var sendMove = function(column, callback) {
    request.post({
        url: API_URL + "/v1/move",
        form: {
            MatchId: MATCH_ID,
            PlayerId: PLAYER_ID,
            Column: column,
            format: "JSON"
        }
    }, function (error, res, body) {
        callback(body);
    });
};

var registerForMatch = function(callback) {
    console.log("Registering for match!" + API_URL + "/v1/matches/" + MATCH_ID + "/register?format=json");
    request.post({
        url: API_URL + "/v1/matches/" + MATCH_ID + "/register?format=json",
        form: {
            MatchName: MATCH_ID,
            PlayerName: PLAYER_ID,
            format: "JSON"
        }
    }, function(error, res, body) {
        callback(body);
    });
};

console.log("Starting listener..." + "\n\n\n");