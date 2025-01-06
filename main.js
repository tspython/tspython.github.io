// main.js
let _serial;
let afc_div_teams;
let nfc_div_teams;
let superbowl_teams;

let afc_conf_teams;
let nfc_conf_teams;

const teams = {
  afc: [
    "chiefs",         // 1 seed
    "bills",        // 2 seed
    "ravens",        // 3 seed
    "texans",        // 4 seed
    "chargers",        // 5 seed  
    "steelers",        // 6 seed
    "broncos"         // 7 seed;
  ],
  nfc: [
    "lions",        // 1 seed
    "eagles",        // 2 seed
    "buccaneers",         // 3 seed
    "rams",        // 4 seed
    "vikings",        // 5 seed
    "football",        // 6 seed
    "packers"          // 7 seed
  ]
};

function make_game() {
  const game = $(
    `<div class="team team-clickable">
            <div class="team_logo">&nbsp;</div>
            <div class="team_layer">&nbsp;</div>
            <div class="seed"></div>
        </div>
        <div class="team team-clickable">
            <div class="team_logo">&nbsp;</div>
            <div class="team_layer">&nbsp;</div>
            <div class="seed"></div>
        </div>`
  );
  return game;
}

function get_serial() {
  let serial = 0;
  $('.game').each(function (_, game) {
    serial *= 3;
    if ($(game).find('.team').eq(0).hasClass('selected')) {
      serial += 1;
    } else if ($(game).find('.team').eq(1).hasClass('selected')) {
      serial += 2;
    }
  });
  return serial;
}

function refresh_bracket(do_set_url) {
  populate_teams();
}

function is_complete() {
  let serial = get_serial();
  let num_flags = 0;
  while (serial > 0) {
    const flag = serial % 3;
    if (flag === 0) {
      return false;
    }
    num_flags++;
    serial = Math.floor(serial / 3);
  }
  return num_flags === 13;
}

window.onpopstate = function (_e) {
  const currentState = history.state;
  if (currentState) {
    select_teams(currentState.serial);
  }
};

function select_teams(serial) {
  _serial = serial;
  $.each($('.game').get().reverse(), function (_, game) {
    $(game).find('.team').removeClass('selected');
    if (_serial) {
      const flag = _serial % 3;
      if (flag === 1) {
        $(game).find('.team').eq(0).addClass('selected');
      } else if (flag === 2) {
        $(game).find('.team').eq(1).addClass('selected');
      }
      _serial = Math.floor(_serial / 3);
    }
  });

  populate_teams();
}

function sort_teams(teams, conf_rank) {
  teams.sort((a, b) => {
    return $.inArray(a, conf_rank) - $.inArray(b, conf_rank);
  });
  return teams;
}

function set_teams(selector, teams) {
  $(selector)
    .find('.team')
    .eq(0)
    .data('team', teams[0] || '');
  $(selector)
    .find('.team')
    .eq(1)
    .data('team', teams[1] || '');
}

function populate_teams() {
  const all_teams = [teams.afc.join(' '), teams.nfc.join(' ')].join(' ');
  $('.team .team_logo').html('');
  $('.team .team_logo').removeClass(all_teams);

  // Wild Card Round
  set_teams('#afc_wc .game:eq(0)', [teams.afc[6], teams.afc[1]]); // 7 vs 2
  set_teams('#afc_wc .game:eq(1)', [teams.afc[5], teams.afc[2]]); // 6 vs 3
  set_teams('#afc_wc .game:eq(2)', [teams.afc[4], teams.afc[3]]); // 5 vs 4

  set_teams('#nfc_wc .game:eq(0)', [teams.nfc[6], teams.nfc[1]]); // 7 vs 2
  set_teams('#nfc_wc .game:eq(1)', [teams.nfc[5], teams.nfc[2]]); // 6 vs 3
  set_teams('#nfc_wc .game:eq(2)', [teams.nfc[4], teams.nfc[3]]); // 5 vs 4

  // Divisional Round
  afc_div_teams = sort_teams(
    [
      teams.afc[0], // 1 seed
      $('#afc_wc .game:eq(0) .selected').data('team'),
      $('#afc_wc .game:eq(1) .selected').data('team'),
      $('#afc_wc .game:eq(2) .selected').data('team')
    ],
    teams.afc
  );

  set_teams('#afc_div .game:eq(0)', [afc_div_teams[3], afc_div_teams[0]]);
  set_teams('#afc_div .game:eq(1)', [afc_div_teams[2], afc_div_teams[1]]);

  nfc_div_teams = sort_teams(
    [
      teams.nfc[0], // 1 seed
      $('#nfc_wc .game:eq(0) .selected').data('team'),
      $('#nfc_wc .game:eq(1) .selected').data('team'),
      $('#nfc_wc .game:eq(2) .selected').data('team')
    ],
    teams.nfc
  );

  set_teams('#nfc_div .game:eq(0)', [nfc_div_teams[3], nfc_div_teams[0]]);
  set_teams('#nfc_div .game:eq(1)', [nfc_div_teams[2], nfc_div_teams[1]]);

  // Conference Championships
  afc_conf_teams = sort_teams(
    [
      $('#afc_div .game:eq(0) .selected').data('team'),
      $('#afc_div .game:eq(1) .selected').data('team')
    ],
    teams.afc
  );

  set_teams('#afc_conf .game', afc_conf_teams.reverse());

  nfc_conf_teams = sort_teams(
    [
      $('#nfc_div .game:eq(0) .selected').data('team'),
      $('#nfc_div .game:eq(1) .selected').data('team')
    ],
    teams.nfc
  );

  set_teams('#nfc_conf .game', nfc_conf_teams.reverse());

  // Super Bowl
  superbowl_teams = [
    $('#afc_conf .game .selected').data('team'),
    $('#nfc_conf .game .selected').data('team')
  ];

  set_teams('#sb_game .game', superbowl_teams);

  $('.champ .team').data(
    'team',
    $('#sb_game .game .selected').data('team') || ''
  );

  // Update team logos and seeds
  $('.team').each(function (_, team) {
    const team_name = $(team).data('team');
    $(team).find('.team_logo').addClass(team_name);

    let seed = teams.afc.indexOf(team_name) + 1;
    if (seed === 0) {
      seed = teams.nfc.indexOf(team_name) + 1;
    }

    if (seed === 0) {
      $(team).find('.seed').hide();
    } else {
      $(team).find('.seed').show().text(seed || '');
    }
  });
}

function init_games() {
  $('div.game').each(function () {
    $(this).append(make_game());
  });
}

function init_selectables() {
  $('.team-clickable')
    .mousedown(function () {
      $(this).removeClass('selected').addClass('selecting');
    })
    .click(function () {
      $(this).removeClass('selecting').siblings().removeClass('selected selecting');
      $(this).addClass('selected');
      refresh_bracket(true);
    });
}

$(function () {
  init_games();
  init_selectables();
  select_teams(_serial);
  if (_serial === 0 && !window.location.toString().match(/\/0\/$/)) {
    // URL handling removed
  }

  $('#reset').click(function () {
    select_teams(0);
  });
});
