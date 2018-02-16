<?php

/**
 * @file
 * Implement an gridstack field.
 */

function gridstack_field_field_widget_info() {
  return array(
    'paragraphs_gridstack' => array(
      'label' => t('Gridstack'),
      'field types' => array('paragraphs'),
      'settings' => array(
        'grid_settings' => array(
          'width' => 12,
          'alwaysShowResizeHandle' => false,
          'cellHeight' => 60,
          'verticalMargin' => 20,
          'height' => 0,
        ),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

function gridstack_field_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings']['grid_settings'];
  $form = array();

  if ($widget['type'] == 'paragraphs_gridstack') {
    $form['grid_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grid settings'),
      '#weight' => 9999,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['grid_settings']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('width'),
      '#default_value' =>  (isset($settings['width'])) ? $settings['width'] : 12,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['alwaysShowResizeHandle'] = array(
      '#type' => 'textfield',
      '#title' => t('alwaysShowResizeHandle'),
      '#default_value' => (isset($settings['alwaysShowResizeHandle'])) ? $settings['alwaysShowResizeHandle'] : 'false',
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['cellHeight'] = array(
      '#type' => 'textfield',
      '#title' => t('cellHeight'),
      '#default_value' => (isset($settings['cellHeight'])) ? $settings['cellHeight'] : 60,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['verticalMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('verticalMargin'),
      '#default_value' => (isset($settings['verticalMargin'])) ? $settings['verticalMargin'] : 20,
      '#size' => 60,
      '#maxlength' => 128,
    );
    $form['grid_settings']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('height'),
      '#default_value' => (isset($settings['height'])) ? $settings['height'] : 0,
      '#size' => 60,
      '#maxlength' => 128,
    );
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function gridstack_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'paragraphs_gridstack':
      return gridstack_field_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $element);
      break;
  }
}

/**
 * Special handling to create form elements for multiple values.
 *
 * Handles generic features for multiple fields:
 * - number of widgets
 * - AHAH-'add more' button
 * - drag-n-drop value reordering
 */
function gridstack_field_field_multiple_value_form($field, $instance, $langcode, $items, &$form, &$form_state, $delta, $original_element) {
  $form['#submit'][] = 'gridstack_field_field_widget_form_submit';
  $field_elements = paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $original_element);
  // Use own theme function.
  $field_elements['#theme'] = 'gridstack_field_field_multiple_value_form';
  // Install Gridstack library on the page.
  drupal_add_library('gridstack_field', 'gridstack', FALSE);

  // .
  if ($form['nid']['#value']) {
    $db_data = gridstack_field_load($form['nid']['#value']);
    cache_set('grid_items' . session_id(), $db_data->data, 'cache', CACHE_PERMANENT);
  }
  return $field_elements;
}

/**
 * .
 */
function gridstack_field_field_widget_form_submit($form, &$form_state) {
  if ($form['nid']['#value']) {
    $cached_data = cache_get('grid_items' . session_id());
    gridstack_field_save($form['nid']['#value'], $cached_data->data);

    // cache_clear_all('grid_items' . session_id());
    // Hack for clearing cache.
    // TODO need to refactor this.
    cache_set('grid_items' . session_id(), NULL, 'cache', REQUEST_TIME);
  }
}

/**
 * Widget form implementation for paragraphs.
 *
 * @param $form
 * @param $form_state
 * @param $field
 * @param $instance
 * @param $langcode
 * @param $items
 * @param $delta
 * @param $element
 *
 * @return array
 */
function gridstack_field_field_widget_form_build(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element = paragraphs_field_widget_form_build($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  return $element;
}
