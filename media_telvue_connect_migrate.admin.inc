<?php

/**
 * Callback for module settings page.
 */
function media_telvue_connect_migrate_settings_form($form, &$form_state) {
  $form = array();

  $form['media_telvue_connect_migrate']['cloudcast_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloudcast ID'),
    '#default_value' => variable_get('cloudcast_id', ''),
    '#description' => 'Cloudcast ID, i.e, T01500',
    '#size' => 120,
  );

  $form['media_telvue_connect_migrate']['connect_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Connect ID'),
    '#default_value' => variable_get('connect_id', ''),
    '#description' => 'Connect ID, i.e, f3efhg7XsR-YcpRlQOnWV8S-4_LF2TVI',
    '#size' => 120,
  );

  return system_settings_form($form);
}
