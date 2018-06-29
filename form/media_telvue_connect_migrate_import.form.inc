<?php

/**
 * Form constructor
 */
function media_telvue_connect_migrate_import_form($form, &$form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Upload XML MRSS Feed file'),
    '#description' => t('.xml upload'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Form validate
 */
function media_telvue_connect_migrate_import_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('xml'), // Validate extensions.
    FILE_EXISTS_REPLACE
  ));

  // If the file passed validation:
  if ($file) {
    // Move the file, into the Drupal file system
    if ($file = file_move($file, 'public://')) {
      // Save the file for use in the submit handler.
      $form_state['values']['file'] = $file;
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * Form submit
 */
function media_telvue_connect_migrate_import_form_submit($form, &$form_state) {
  // Get file object
  $file_object = $form_state['values']['file'];

  // Get xml file
  // @todo check if this works on Pantheon?!
  $xml_file = drupal_realpath($file_object->uri);

  // Get file as object
  $xml = new SimpleXMLElement($xml_file, null, true);

  // Get namespaces
  $namespaces = $xml->getNamespaces(true);

  foreach($xml->xpath('//item') as $item) {
    // Get data in cloudcast: namespace
    $cloudcast = $item->children($namespaces['cloudcast']);
    // Get data in media: namespace.
    $media = $item->children($namespaces['media']);

    // Build data array
    $data[] = array(
      'cloudcast_video_id' => (string) $cloudcast->{'content-file-id'},
      'connect_video_id' => (string) $cloudcast->id,
    );
  }

  // Truncate any existing data
  db_truncate('media_telvue_connect_migrate_legacy')->execute();

  // Iterate over our data and write to the DB
  foreach ($data as $item) {
    if (empty($item['cloudcast_video_id'])) {
      continue;
    }
    else {
      drupal_write_record('media_telvue_connect_migrate_legacy', $item);
    }
  }
}
