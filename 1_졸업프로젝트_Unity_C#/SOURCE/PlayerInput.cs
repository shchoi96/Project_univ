using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Photon.Pun;

public class PlayerInput : MonoBehaviourPun
{
    PlayerCharacter player; // 플레이어 정보

    // 움직임 입력값
    public float horizontalMove;
    public float verticalMove;
    // 움직임 외 입력값 ( 공격, 스킬 등 )
    public enum keyboard { attack, skill, avoid, heal, shield, potion, nothing };
    public keyboard keyboardInput;
    private keyboard touch;
    //JoyStick
    public JoyStick stick;

    // 초기화
    private void Start()
    {
        player = GetComponent<PlayerCharacter>();
        stick = GameObject.Find("JoyStickCanvas").GetComponentInChildren<JoyStick>();
        keyboardInput = keyboard.nothing;
        touch = keyboard.nothing;
    }
    // 매 프레임 플레이어 입력을 감지
    private void Update()
    {
        // 로컬 플레이어가 아닌 경우 입력을 받지 않음
        if (!photonView.IsMine)
        { 
            return;
        }
        // 로컬 플레이어가 죽은 상태에서는 입력을 받지 않음
        if(player.dead)
        {
            horizontalMove = 0;
            verticalMove = 0;
            keyboardInput = keyboard.nothing;
            return;
        }
        // 움직임 입력 감지
        if (stick.JoyVec != Vector3.zero)
        {
            horizontalMove = stick.JoyVec.x;
            verticalMove = stick.JoyVec.y;
        }
        else
        {
            horizontalMove = Input.GetAxis("Horizontal");
            verticalMove = Input.GetAxis("Vertical");
        }
        // 움직임 외 입력 감지
        if (Input.GetKeyDown(KeyCode.Z) || touch == keyboard.attack)
        {
            keyboardInput = keyboard.attack;
        }
        else if (Input.GetKeyDown(KeyCode.X) || touch == keyboard.skill)
        {
            keyboardInput = keyboard.skill;
        }
        else if (Input.GetKeyDown(KeyCode.C) || touch == keyboard.heal)
        {
            keyboardInput = keyboard.heal;
        }
        else if (Input.GetKeyDown(KeyCode.V) || touch == keyboard.shield)
        {
            keyboardInput = keyboard.shield;
        }
        else if (Input.GetKeyDown(KeyCode.B) || touch == keyboard.potion)
        {
            keyboardInput = keyboard.potion;
        }
        else if (Input.GetKeyDown(KeyCode.Space) || touch == keyboard.avoid)
        {
            keyboardInput = keyboard.avoid;
        }
        else
        {
            keyboardInput = keyboard.nothing;
        }
        touch = keyboard.nothing;

    }
    public void AttackButton()
    {
        touch = keyboard.attack;
    }
    public void SkillButton()
    {
        touch = keyboard.skill;
    }
    public void HealButton()
    {
        touch = keyboard.heal;
    }
    public void ShieldButton()
    {
        touch = keyboard.shield;
    }
    public void PotionButton()
    {
        touch = keyboard.potion;
    }
    public void AvoidButton()
    {
        touch = keyboard.avoid;
    }
}
