using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Photon.Pun;

public class GameManager : MonoBehaviour
{
    // 싱글턴 객체
    public static GameManager gameManager;
    // 메인 카메라
    public static GameObject mainCamera;
    // Player & Raid Monster
    public static List<GameObject> players = new List<GameObject>();
    public static GameObject monster;
    // UI
    private GameObject[] otherPlayerName = new GameObject[2]; // 팀원의 닉네임과 체력 UI
    private GameObject[] otherPlayerHp = new GameObject[2];
    public GameObject continewButton;
    public GameObject continewMessage;

    private void Awake()
    {
        if (gameManager == null)
            gameManager = this;
    }

    void Start()
    {
        Screen.SetResolution(2160, 1080, true);
        mainCamera = GameObject.Find("_Main Camera1");

        otherPlayerName[0] = GameObject.Find("PlayerHp/OtherPlayerName1");
        otherPlayerName[1] = GameObject.Find("PlayerHp/OtherPlayerName2");
        otherPlayerHp[0] = GameObject.Find("PlayerHp/OtherPlayerHp1");
        otherPlayerHp[1] = GameObject.Find("PlayerHp/OtherPlayerHp2");
        continewButton = GameObject.Find("SystemMessage/ContinewButton");
        continewButton.SetActive(false);
    }

    private void FixedUpdate()
    {
        InitPlayersUI();
    }

    // 자신의 캐릭터와 팀원 캐릭터의 체력 바를 현재 체력에 맞게 갱신
    private void InitPlayersUI()
    {
        int i = 0;
        foreach (GameObject otherPlayer in players)
        {
            if(otherPlayer.GetComponent<PhotonView>().IsMine)
            {
                otherPlayer.GetComponent<PlayerCharacter>().slider.value = otherPlayer.GetComponent<PlayerCharacter>().health;
            }
            else
            {
                otherPlayerHp[i].SetActive(true);
                otherPlayerHp[i].GetComponent<Slider>().maxValue = otherPlayer.GetComponent<PlayerCharacter>().InitHealth;
                otherPlayerHp[i].GetComponent<Slider>().value =    otherPlayer.GetComponent<PlayerCharacter>().health;
                otherPlayerName[i].SetActive(true);
                otherPlayerName[i].GetComponent<Text>().text = otherPlayer.GetComponent<PlayerCharacter>().nickName;
                ++i;
            }
            if (otherPlayer.GetComponent<PlayerCharacter>().health <= 0)
                players.Remove(otherPlayer);
        }
        for(int j=i;j<2;++j)
        {
            otherPlayerHp[j].SetActive(false);
            otherPlayerName[j].SetActive(false);
        }
    }
    // UI 시스템 메시지 함수
    public IEnumerator SystemMessage(string message)
    {
        Text text = GameObject.Find("SystemMessage").GetComponentInChildren<Text>();
        text.text = message;
        yield return new WaitForSeconds(1.5f);
        text.text = "";
    }
    // 재시작 UI -- 다른 팀원
    public IEnumerator ShowContinew()
    {
        continewMessage.SetActive(true);
        yield return new WaitForSeconds(5f);
        continewButton.SetActive(true);
    }
}
