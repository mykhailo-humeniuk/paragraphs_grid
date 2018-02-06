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
        'width' => 12,
        'alwaysShowResizeHandle' => false,
        'cellHeight' => 60,
        'verticalMargin' => 20,
        'height' => 0,
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
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
  $field_elements = paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state, $delta, $original_element);
  // Use own theme function.
  $field_elements['#theme'] = 'gridstack_field_field_multiple_value_form';
  // Install Gridstack library on the page.
  drupal_add_library('gridstack_field', 'gridstack', FALSE);
  return $field_elements;
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

/**
 * Implements hook_form_field_ui_field_edit_form_alter().
 */
function gridstack_field_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  if ($form['#field']['type'] == 'paragraphs' && $form['#instance']['widget']['type'] == 'paragraphs_gridstack') {
    $settings = $form['#instance']['widget']['settings'];

    $form['grid_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grid settings'),
      '#weight' => 5,
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

    array_unshift($form['#submit'], 'gridstack_field_ui_field_edit_form_submit');
  }
}

/**
 * @param $form
 * @param $form_state
 *
 * Submit function for saving grid settings into widget.
 */
function gridstack_field_ui_field_edit_form_submit($form, &$form_state) {
  $settings = $form_state['values']['instance']['widget']['settings'];
  $settings['width'] = $form_state['values']['width'];
  $settings['cellHeight'] = $form_state['values']['cellHeight'];
  $settings['height'] = $form_state['values']['height'];
  $settings['alwaysShowResizeHandle'] = $form_state['values']['alwaysShowResizeHandle'];
  $settings['verticalMargin'] = $form_state['values']['verticalMargin'];
}

